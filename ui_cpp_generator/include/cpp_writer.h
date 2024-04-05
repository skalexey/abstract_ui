#pragma once

#include <cassert>
#include <iterator>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include <unordered_set>
#include <vl_visitor.h>
#include <vl/var_ptr.h>
#include <vl/TypeResolver.h>
#include <utils/entity.h>
#include <utils/ordered_map.h>
#include <utils/string_utils.h>
#include "cppgen_fwd.h"
#include "cppwriter_fwd.h"

namespace utils
{
	namespace ui
	{
		// Class that isolates every entity that has a VL data linked to it. It represents a VL variable with name and parent.
		class var : public entity
		{
		public:
			using base = entity;

			virtual void init(const vl::VarPtr& data, const std::string& name, var* parent = nullptr)
			{
				m_data = data;
				m_name = name;
				m_parent = parent;
			}
			var* parent() const {
				return m_parent;
			}
			void set_parent(var* parent) {
				m_parent = parent;
			}
			bool is_root() const {
				return parent() == nullptr;
			}
			const std::string& get_name() const {
				return m_name;
			}
			const vl::VarPtr& get_data() const {
				return m_data;
			}
			bool has_proto() const;
			std::string get_proto_type_name(const vl::TypeResolver& type_resolver) const;

			std::string get_path(const std::string& delimeter = ".") const {
				std::string result = get_name();
				auto ctx = parent();
				while (ctx) {
					result = (ctx->get_name() + delimeter) + result;
					ctx = ctx->parent();
				}
				return result;
			}

			bool is_reference() const;

		protected:
			vl::VarPtr& data() {
				return m_data;
			}

		private:
			vl::VarPtr m_data;
			std::string m_name;
			var* m_parent;
		};

		// Class that represents an instance of a type
		class value : public var
		{
		public:
			using base = var;

			void init(const vl::VarPtr& data, const std::string& name, var* parent = nullptr) override;

			value_const_ptr get(const std::string_view& name_or_ref) const;

			value_ptr get(const std::string& name_or_ref) {
				return std::const_pointer_cast<value>(std::as_const(*this).get(name_or_ref));
			}

			type_ptr get_type() const {
				if (is_reference()) {
					// Try interpreting it as a reference relative to the parent
					auto parent = this->parent();
					if (parent) {
						auto& ref = get_data()->as<vl::String>().Val();
						auto field = parent->as_ptr<value>()->get(ref);
						assert(field);
						if (field) {
							auto type = field->get_type();
							return type;
						}
					}
				}
				return m_type;
			}

		protected:
			void set_type(const type_ptr& type) {
				m_type = type;
			}

		private:
			type_ptr m_type;
			vl::VarPtr m_properties;
		};
	
		// Type
		class type : public entity
		{
		public:
			virtual void init(const std::string& type_name) {
				m_name = type_name;
				m_include_path = "\"" + type_name + ".h\"";
			}

			const std::string& get_include_path() const {
				return m_include_path;
			}

			const std::string& get_name() const {
				return m_name;
			}

		protected:
			void set_include_path(const std::string& include_path) {
				m_include_path = include_path;
			}

		private:
			std::string m_include_path;
			std::string m_name; // It may differ from var::m_name if it is a type of a field.
		};

		class embedded_type : public virtual type {};

		class system_type : public virtual type
		{
		public:
			using base = type;

			void init(const std::string& name) override
			{
				type::init(name);
				// If name contains "layout", then include it from widgets/layouts folder
				std::string prefix = "";
				if (name.find("layout") != std::string::npos)
					prefix = "layouts/";
				else if (name.find("dialog_") != std::string::npos)
					prefix = "dialogs/";
				set_include_path("<abstract_ui/final/widgets/" + prefix + name + ".h>");
			}
		};

		class class_desc : public virtual type {};

		struct type_print_context
		{
			cpp_writer& writer;
			const class_desc* class_ref;
			std::ofstream* file = nullptr;
			int indent_level = 0;
			type_print_context* parent_context = nullptr;

			std::string cpp_scope(const std::string& suffix = "") const {
				std::string result;
				auto ctx = this;
				while (ctx) {
					result = (ctx->class_ref->get_name() + "::") + result;
					ctx = ctx->parent_context;
				}
				if (!suffix.empty())
					result += (suffix + "::");
				return result;
			}
		};

		class custom_type : public virtual type
		{
		public:
			virtual int print(type_print_context& ctx) const = 0;
		};

		// structure
		class structure : public custom_type, public class_desc
		{
			friend class cpp_writer;

		public:
			using base = type;

			// Field is just the same as value, but to avoid renaming in the future in the case of reusing value functional.
			struct print_data;
			class field : public value {
			public:
				void collect_data_recursion(
					type_print_context& ctx
					, print_data& pdata
					, int recursion_level
				);
			};
			using field_ptr = std::shared_ptr<field>;

			using fields_list = std::vector<field_ptr>;
			using fields_map_view = utils::ordered_hashmap_view<std::string, field_ptr>;
			using types_map = utils::ordered_hashmap<std::string, type_ptr>;
			
			void init(const std::string& type_name, var* var);

			bool is_empty() {
				return get_name().empty() && m_fields.empty();
			}
			const field_ptr& add_field(const std::string& name, const field_ptr& field) {
				auto& res = m_fields.add(name, field);
				assert(m_var);
				field->set_parent(m_var);
				assert(res.second && "Several fields with the same name?");
				return (*res.first).second;
			}
			const field_ptr* get_field(const std::string_view& field_name) const {
				// TODO: try to get by the string_view without a conversion
				auto str = std::string(field_name);
				auto it = m_fields.find(str);
				if (it == m_fields.end())
					return nullptr;
				return &(*it).second;
			}
			void set(const std::string& field_name, const field_ptr& field) {
				m_fields.set(field_name, field);
			}
			int print(type_print_context& ctx) const override;
			int print_fwd(type_print_context& ctx) const;

			struct print_data
			{
				std::string proto_id; // Not used yet, but may be useful in the future
				types_map subclasses;
				fields_list fields;
				std::vector<custom_type_ptr> custom_types;
				//fields_map primitives; // Not used yet, but may be useful in the future
				std::unordered_set<std::string> includes;
				std::unordered_set<std::string> generated_types_to_include;
			};

			struct print_definitions_context
			{
				type_print_context& ctx;
				const print_data* pdata = nullptr;
			};
			void print_definitions(print_definitions_context& ctx) const;
			int collect_data(print_data& data, type_print_context& ctx) const;
			fields_map_view get_fields_map() const {
				return fields_map_view(m_fields);
			}

			type_ptr get_base_type() const {
				return m_base_type;
			}

		private:
			utils::ordered_hashmap<std::string, field_ptr> m_fields;
			vl::VarPtr m_properties;
			vl::VarPtr m_parameters;
			type_ptr m_base_type;
			var* m_var;
		};

		// This class is needed to incapsulate the list element creation to keep value::set_type() protected.
		class list_element : public value
		{
		public:
			using base = value;
			void init(const vl::VarPtr& data, const std::string& name, var* parent = nullptr) override;
		};

		// list
		class list : public embedded_type, public custom_type
		{
		public:
			using base = embedded_type;

			void init(var* var);

			int print(type_print_context& ctx) const override;

		private:
			list_element_ptr m_element;
			var* m_var;
		};

		// cpp_writer
		class cpp_writer : public vl::Visitor
		{
			// These classes use incapsulated print context of the writer
			friend class var;
			friend class structure;
			friend class list;
			
		protected:
			struct global_print_context
			{
				std::ofstream fwd_f;
				std::unordered_set<std::string> printed_classes;
			};
			
		public:
			cpp_writer(const vl::TypeResolver& type_resolver, const cppw_params& params = cppw_params());
			// Visitor interface
			bool VisitNull(const vl::NullVar& var, const char* name) override;
			bool VisitBool(const vl::BoolVar& var, const char* name) override;
			bool VisitNumber(const vl::NumberVar& var, const char* name) override;
			bool VisitString(const vl::StringVar& var, const char* name) override;
			bool VisitPointer(const vl::PointerVar& var, const char* name) override;
			bool VisitObject(const vl::ObjectVar& var, const char* name) override;
			bool EndVisitObject(const vl::ObjectVar& var, const char* name) override;
			bool VisitList(const vl::ListVar& var, const char* name) override;
			bool EndVisitList(const vl::ListVar& var, const char* name) override;

		public:
			const cppw_params& get_params() const { return m_params; }
			const vl::TypeResolver& get_type_resolver() const { return m_type_resolver; }

		protected:
			structure_ptr process_root_type(const vl::VarPtr& data, const std::string& var_name);
			void process_content(const vl::Object& object);
			// bool add_proto(const vl::ObjectVar& value);
			// bool add_member(const var_ptr& val, const char* name);
			var_ptr get_current_container();
			void push_current_container(const var_ptr& val);
			bool pop_current_container();
			global_print_context& print_context() {
				return m_print_context;
			}
		
		protected:
			void begin_writing_fwd();
			void end_writing_fwd();
			void print_fwd(const std::string& type_name);
			
		private:
			std::vector<var_ptr> m_stack;
			var_ptr m_root;
			// TODO: combine with root
			std::unordered_map<std::string, value_ptr> m_root_types;
			cppw_params m_params;
			vl::TypeResolver m_type_resolver;
			global_print_context m_print_context;
		};
	}
}