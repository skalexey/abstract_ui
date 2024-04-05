#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <cstring>
#include <cassert>
#include <vl.h>
#include <vl/TypeResolver.h>
#include <utils/file_utils.h>
#include <utils/log.h>
#include <abstract_ui/widget_factory.h>
#include "cpp_writer.h"

LOG_TITLE("cpp_writer")
SET_LOCAL_LOG_LEVEL(debug)

namespace
{
	bool is_reserved_name(const std::string& name)
	{
		static std::unordered_set<std::string> reserved_names = {
			"proto",
			"base",
			"type",
			"parameters",
			"properties",
			"preferred_name",
			"list",
			"element"
		};
		return reserved_names.find(name) != reserved_names.end();
	}

	// template <typename Container_t, typename Element_t>
	// bool add_unique(Container_t& container, const Element_t& name)
	// {
	// 	if (container.find(name) != container.end())
	// 		return false;
	// 	container.emplace(name);
	// 	return true;
	// }

	#define PRINT_INDENT(file, level) \
		for (int i = 0; i < level; i++) \
			file << "\t"

	#define PRINT(file, data) file << data

	#define INIT_PRINT(file, indent_level) \
		auto& pr_file = file; \
		auto pr_ind_level = indent_level \
	
	#define BEGIN_PRINT_LINE PRINT_INDENT(pr_file, pr_ind_level)

	#define END_PRINT_LINE PRINT(pr_file, "\n")

	#define PRINT_LINE(data) { \
		BEGIN_PRINT_LINE; \
		PRINT(pr_file, data); \
		END_PRINT_LINE; }

	#define PRINT_LINE_BREAK PRINT_LINE("")

	#define PRINT_INDENT_DECREASE \
		pr_ind_level--

	#define PRINT_INDENT_INCREASE \
		pr_ind_level++

	#define INDENT_LEVEL pr_ind_level
	
	#define PRINT_SCOPE_BEGIN \
		PRINT_LINE("{"); \
		PRINT_INDENT_INCREASE;

	#define PRINT_SCOPE_END \
		PRINT_INDENT_DECREASE; \
		PRINT_LINE("}");
	
	#define PRINT_SCOPE_END_SEMI \
		PRINT_INDENT_DECREASE; \
		PRINT_LINE("};");

	#define CLASS_DATA_INITIALIZER (m_var->has_proto() ? (m_var->get_proto_type_name(ctx.writer.get_type_resolver()) + "(data)") : "m_data(data)")
	#define CLASS_DATA_INITIALIZER_2 (m_var->has_proto() ? (m_var->get_proto_type_name(ctx.writer.get_type_resolver()) + "(vl::MakePtr(data))") : "m_data(vl::MakePtr(data))")

	#define M_DATA \
		(pdata.proto_id.empty() ? "m_data" : "_data_()")

	// Data checks
	#define PRINT_DATA_CHECK(return_ex) \
		PRINT_LINE("if (!" << M_DATA << ")"); \
		PRINT_INDENT_INCREASE; \
		PRINT_LINE("return" << return_ex << ";"); \
		PRINT_INDENT_DECREASE;

	#define PRINT_DATA_IS_OBJECT_CHECK(return_ex) \
		PRINT_LINE("if (!" << M_DATA << "->is<vl::Object>())"); \
		PRINT_INDENT_INCREASE; \
		PRINT_LINE("return" << return_ex << ";"); \
		PRINT_INDENT_DECREASE;

	// Variables declarations
	#define PRINT_DATA_OBJ \
		PRINT_LINE("auto& data_obj = " << M_DATA << "->as<vl::Object>();");

	#define VARIABLE_DECLARATION(t, n, v) \
		t << " " << n << " = " << v << ";";

	#define STATIC_VARIABLE_DECLARATION(t, n, v) \
		"static " << VARIABLE_DECLARATION(t, n, v)

	#define PRINT_DATA_OBJECT_WITH_CHECKS(return_ex) \
		PRINT_DATA_CHECK(return_ex); \
		PRINT_DATA_IS_OBJECT_CHECK(return_ex); \
		PRINT_DATA_OBJ;

	#define DATA_VAR_NAME(fn) \
		"data_" << fn

	#define PRINT_DATA_GET(fn) \
		PRINT_LINE("auto& " << DATA_VAR_NAME(fn) << " = data_obj.Get(\"" << fn << "\");");
	

	#define PRINT_RETURN_DATA_METHOD_UNQUOTED(fn, m, suffix) \
		PRINT_LINE("return data_obj." << m << "(" << fn << ")" << suffix << ";");

	// Typed data
	#define PRINT_DATA_IS_TYPE_CHECK(t, fn, return_ex); \
		PRINT_LINE("if (!" << DATA_VAR_NAME(fn) << ".Is" << t << "())"); \
		PRINT_INDENT_INCREASE; \
		PRINT_LINE("return" << return_ex << ";"); \
		PRINT_INDENT_DECREASE;

	#define PRINT_DATA_TYPE(t, fn); \
		PRINT_LINE("auto& " << DATA_VAR_NAME(fn) << "_" << t " = " << DATA_VAR_NAME(fn) << ".As" << t << "()"; );

	#define PRINT_DATA_RETURN_AS_TYPE(t, fn, suffix); \
		PRINT_LINE("return " << DATA_VAR_NAME(fn) << ".As" << t << "()" << suffix << ";");

	#define PRINT_DATA_TYPE_WITH_CHECKS(t, fn) \
		PRINT_DATA_OBJECT_WITH_CHECKS(" empty_val"); \
		PRINT_DATA_GET(fn); \
		PRINT_DATA_IS_TYPE_CHECK(t, fn, " empty_val"); \
		PRINT_DATA_TYPE(t, fn);

	#define PRINT_DATA_TYPE_RETURN_WITH_CHECKS(t, fn, suffix) \
		PRINT_DATA_OBJECT_WITH_CHECKS(" empty_val"); \
		PRINT_DATA_GET(fn); \
		PRINT_DATA_IS_TYPE_CHECK(t, fn, " empty_val"); \
		PRINT_DATA_RETURN_AS_TYPE(t, fn, suffix);

	#define PRINT_DATA_RETURN_WITH_CHECKS_UNQUOTED(fn, m, return_ex) \
		PRINT_DATA_OBJECT_WITH_CHECKS(return_ex); \
		PRINT_RETURN_DATA_METHOD_UNQUOTED(fn, m, "");
}

namespace
{
	#define CLASS_CPP_SCOPE (ctx.cpp_scope())
	#define CLASS_DEF_CONSTRUCTOR_SIGNATURE get_name() << "()"
	#define CLASS_DEF_CONSTRUCTOR_DECL CLASS_DEF_CONSTRUCTOR_SIGNATURE << " = default;"
	#define INIT_METHOD_SIGNATURE(scope) "void " << scope << "init(utils::ui::node* owner)"
	#define INIT_METHOD_DECLARATION INIT_METHOD_SIGNATURE("")

	#define METHOD_SIGNATURE(type, name, args, scope, suffix) \
		type << " " << scope << name << "(" << args << ")" << suffix
	#define MEHTOD_DECLARATION(type, name, args, suffix) METHOD_SIGNATURE(type, name, args, "", suffix) << ";"
	#define METHOD_DEFINITION(type, name, args, suffix) METHOD_SIGNATURE(type, name, args, CLASS_CPP_SCOPE, suffix)

	#define CLASS_GETTER_SIGNATURE(prefix, fn, c, scope, suffix) \
		METHOD_SIGNATURE(prefix << c->as_ptr<structure>()->get_name() << "& ", "" << fn, "", scope, suffix)
	#define CLASS_GETTER_DECLARATION(prefix, fn, c, suffix) CLASS_GETTER_SIGNATURE(prefix, fn, c, "", suffix) << ";"
	#define CLASS_GETTER_DEFINITION(prefix, fn, c, suffix) CLASS_GETTER_SIGNATURE(prefix, fn, c, CLASS_CPP_SCOPE, suffix)

	#define PRINT_SETTER_DEFINITION(fn, t) \
		PRINT_LINE(METHOD_DEFINITION("void", "set_" + fn, t << " value", "")); \
		PRINT_SCOPE_BEGIN; \
		PRINT_DATA_OBJECT_WITH_CHECKS(""); \
		PRINT_LINE("data_obj.Set(\"" << fn << "\", value);"); \
		PRINT_SCOPE_END;
}

namespace
{
	const std::unordered_set<std::string> restricted_names = { "alignas", "alignof", "and", "and_eq", "asm", "atomic_cancel", "atomic_commit", "atomic_noexcept", "auto", "bitand", "bitor", "bool", "break", "case", "catch", "char", "char8_t", "char16_t", "char32_t", "class", "compl", "concept", "const", "consteval", "constexpr", "constinit", "const_cast", "continue", "co_await", "co_return", "co_yield", "decltype", "default", "delete", "do", "double", "dynamic_cast", "else", "enum", "explicit", "export", "extern", "false", "float", "for", "friend", "goto", "if", "inline", "int", "long", "mutable", "namespace", "new", "noexcept", "not", "not_eq", "nullptr", "operator", "or", "or_eq", "private", "protected", "public", "reflexpr", "register", "reinterpret_cast", "requires", "return", "short", "signed", "sizeof", "static", "static_assert", "static_cast", "struct", "switch", "synchronized", "template", "this", "thread_local", "throw", "true", "try", "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual", "void", "volatile", "wchar_t", "while", "xor", "xor_eq" };

	bool is_restricted_name(const std::string& name)
	{
		return restricted_names.find(name) != restricted_names.end();
	}

	std::string process_vl_type_name(std::string type_name) {
		if (is_restricted_name(type_name))
			return type_name + "_t";
		std::string::size_type n = 0;
		const std::string s = ".";
		const std::string t = "::";
		while ((n = type_name.find(s, n)) != std::string::npos)
		{
			type_name.replace(n, s.size(), t);
			n += t.size();
		}
		return type_name;
	}

	std::string process_field_name(std::string field_name) {
		if (is_restricted_name(field_name))
			return field_name + "_f";
		return field_name;
	}

	bool is_path(const std::string& type_name) {
		return type_name.find_first_of(":.") != std::string::npos;
	}
}

namespace utils
{
	namespace ui
	{
		// recursion: < 0 - no recursion, otherwise it is a recursion level incrementing every iteration
		void foreach_field(
			const structure& v
			, const std::function<void(const std::string&, const structure::field_ptr&, int)>& pred
			, int recursion = -1
		)
		{
			auto view = v.as_ptr<structure>()->get_fields_map();
			for (auto&& f : view)
			{
				if (recursion >= 0)
					if (auto field_type = f.second->get_type()->as_ptr<structure>())
						foreach_field(*field_type, pred, recursion + 1);
				pred(f.first, f.second, recursion);
			}
			// There is no use for JSON arrays at the moment
			//else if (v.is<list>())
			//	for (auto& f : v.as_ptr<list>()->get_fields())
			//	{
			//		if (recursion >= 0)
			//			foreach_field(*f, pred, recursion + 1);
			//		pred("", f, recursion);
			//	}
		}

		class type_factory
		{
			static std::unordered_map<std::string, type_ptr> m_types;

			// Shared code for both register_custom_type functions
			struct register_type_impl_1_result
			{
				std::string full_type_name;
				int retcode = 0;
			};
			static register_type_impl_1_result register_type_impl_1(const vl::VarPtr& data, const std::string& name, var* parent = nullptr)
			{
				register_type_impl_1_result result;
				result.full_type_name = parent ? parent->get_path() + name : name;
				if (auto preferred_name_var = data.as<vl::Object>().Get("preferred_name"))
					result.full_type_name = preferred_name_var.as<vl::String>().Val();
				else if (name == "element")
					if (parent && parent->is<list>())
						result.full_type_name += "_element";
				if (result.full_type_name.empty())
				{
					LOG_ERROR("An empty type name has been generated for the field '" << name << "'");
					result.retcode = 1;
				}
				else if (is_system_name(result.full_type_name))
				{
					LOG_ERROR("The type name '" << result.full_type_name << "' is a reserved system type name");
					result.retcode = 2;
				}
				else if (is_embedded_type_name(result.full_type_name))
				{
					LOG_ERROR("The type name '" << result.full_type_name << "' is a reserved embedded type name");
					result.retcode = 3;
				}
				else if (is_restricted_name(result.full_type_name))
				{
					LOG_ERROR("The type name '" << result.full_type_name << "' is a C++ reserved name");
					result.retcode = 4;
				}
				else
				{
					auto it = m_types.find(result.full_type_name);
					if (it != m_types.end())
					{
						LOG_ERROR("The type name '" << result.full_type_name << "' is already registered");
						result.retcode = 5;
					}
				}
				return result;
			}

			template <typename T>
			static type_ptr get_parameterized_type(var* var)
			{
				auto& full_type_name = var->get_path();
				// Find or create a custom list type with the name of the field.
				if (auto type = find_type(full_type_name))
					return type;
				auto type = std::make_shared<T>();
				type->init(var);
				m_types[full_type_name] = type;
				return type;
			}

			template <typename T>
			static type_ptr get_parameterized_type(const vl::VarPtr& data, const std::string& var_name, var* parent = nullptr)
			{
				auto& full_type_name = var->get_path();
				// Find or create a custom list type with the name of the field.
				if (auto type = find_type(full_type_name))
					return type;
				auto type = m_types[full_type_name] = std::make_shared<T>();
				type->init(data, var_name, parent);
				return type;
			}

		public:
			static structure_ptr register_custom_type(var* var)
			{
				auto res = register_type_impl_1(var->get_data(), var->get_name(), var->parent());
				if (res.retcode != 0)
					return nullptr;
				auto new_type = std::make_shared<structure>();
				new_type->init(res.full_type_name, var);
				m_types[res.full_type_name] = new_type;
				return new_type;
			}

			static bool is_system_name(const std::string& name)
			{
				// TODO: generalize this list in abstract_ui
				static std::unordered_set<std::string> system_types = {
					"widget",
					"vertical_layout",
					"horizontal_layout",
					"auto_layout",
					"button",
					"dialog",
					"menu",
					"menu_item",
					"menu_list",
					"menu_bar",
					"label",
					"text",
					"text_input",
					"image",
					"dialog_input_text",
					"dialog_message",
					"dialog_with_buttons",
					"dialog_yes_no",
				};
				return system_types.find(name) != system_types.end();
			}

			static bool is_embedded_type_name(const std::string& name)
			{
				static std::unordered_set<std::string> embedded_types = {
					"list",
				};
				return embedded_types.find(name) != embedded_types.end();
			}

			static bool is_parameterized_type(const std::string& name)
			{
				return name == "list";
			}

			using creator_t = std::function<type_ptr(var*)>;
			static creator_t get_creator(const std::string& type_name)
			{
				static std::unordered_map<std::string, creator_t> creators = {
					{
						"list", [](var* var) {
							return get_parameterized_type<list>(var);
						}
					},
				};
				auto it = creators.find(type_name);
				if (it != creators.end())
					return it->second;
				return nullptr;
			}

			static type_ptr get_type(const std::string& type_name, var* var)
			{
				if (is_parameterized_type(type_name))
				{
					if (auto creator = get_creator(type_name))
						return creator(var);
					else
						LOG_ERROR("The parameterized type '" << type_name << "' is not registered");
				}
				return find_type(type_name);
			}

			static type_ptr find_type(const std::string& type_name)
			{
				auto it = m_types.find(type_name);
				if (it != m_types.end())
					return it->second;
				if (is_system_name(type_name))
				{
					auto type = std::make_shared<system_type>();
					type->init(type_name);
					return m_types[type_name] = type;
				}
				else if (is_embedded_type_name(type_name))
				{
					if (type_name == "list")
					{
						LOG_ERROR("The 'list' type is not complete. Use type_factory::get_type() to specify the element type through data");
						return nullptr;
					}
					else
					{
						auto type = std::make_shared<embedded_type>();
						type->init(type_name);
						return m_types[type_name] = type;
					}
				}
				return nullptr;
			}
		};

		std::unordered_map<std::string, type_ptr> type_factory::m_types;

		std::string process_new_type_name(std::string type_name) {
			if (type_factory::is_system_name(type_name) || type_factory::is_embedded_type_name(type_name))
			{
				LOG_ERROR("The type name '" << type_name << "' is reserved");
				assert(false);
				return "";
			}
			if (is_restricted_name(type_name))
				return type_name + "_t";
			return type_name;
		}

		// Begin of cpp_writer
		cpp_writer::cpp_writer(const vl::TypeResolver& type_resolver, const cppw_params& params)
			: m_params(params)
			, m_type_resolver(type_resolver)
		{}

		// bool cpp_writer::add_member(const var_ptr& val, const char* name)
		// {
		// 	bool is_root = false;
		// 	if (!get_current_container())
		// 	{
		// 		if (m_root == nullptr)
		// 		{
		// 			if (val->is<structure>() && val->as_ptr<structure>()->is_root())
		// 				m_root = val;
		// 			else
		// 			{
		// 				LOCAL_WARNING("Skipping field '" << name << "' on the root level");
		// 				return false;
		// 			}
		// 		}
		// 		is_root = true;
		// 		push_current_container(m_root);
		// 	}
		// 	auto parent = get_current_container();

		// 	// Ignore specified branch
		// 	if (!m_params.ignore.empty())
		// 		if (m_type_resolver.GetTypeId(parent->get_data().as<vl::Object>()) == m_params.ignore)
		// 			return false;

		// 	// Process containers
		// 	if (parent->is<structure>())
		// 	{
		// 		auto parent_obj = parent->as_ptr<structure>();
		// 		if ((val->is<structure>() && !is_root) || !val->is<structure>())
		// 		{
		// 			assert(name); // Any property if an object should have it's name
		// 			auto name_str = (name ? name : "");
		// 			var_ptr new_val(nullptr);
		// 			if (auto r = parent_obj->get_field(name_str))
		// 			{
		// 				if (name_str != std::string("proto")) // Ignore the proto
		// 					parent_obj->set(name_str, *r);
		// 				new_val = *r;
		// 			}
		// 			else
		// 			{
		// 				if (name_str != std::string("proto")) // Ignore the proto
		// 					new_val = parent_obj->add_field(name_str, val->as_ptr<field>());
		// 				else
		// 					new_val = val;
		// 			}
		// 			if (new_val->is<structure>() || new_val->is<list>())
		// 				push_current_container(new_val);
		// 		}
		// 	}
		// 	// There is no use for JSON arrays at the moment
		// 	//else if (parent->is<list>())
		// 	//{
		// 	//	auto current_val_list = parent->as_ptr<list>();
		// 	//	auto& new_val = current_val_list->add_field(val);
		// 	//	if (new_val->is<structure>() || new_val->is<list>())
		// 	//		push_current_container(new_val);
		// 	//}
		// 	else
		// 		throw "cpp_writer: unsupported container type for adding elements";
			
		// 	return true;
		// }

		// bool cpp_writer::add_proto(const vl::ObjectVar& value)
		// {
		// 	auto current_val = get_current_container();
		// 	if (m_params.use_proto_refs)
		// 	{
		// 		auto type_id = m_type_resolver.GetTypeId(value);
		// 		auto val = std::make_shared<structure>(vl::MakePtr(value), process_new_type_name(type_id));
		// 		add_member(val, "proto");
		// 		return false; // Don't visit nested 'proto' object
		// 	}
		// 	// Prototypes are not in use at the moment
		// 	//else if (m_params.merge_with_proto)
		// 	//{
		// 	//	// Put everything from 'proto' into the current container
		// 	//	push_current_container(current_val);
		// 	//	if (m_params.store_type_id)
		// 	//	{
		// 	//		auto val = std::make_shared<primitive_type>(vl::MakePtr(value));
		// 	//		add_member(val->as_ptr<var>(), "typeid");
		// 	//	}
		// 	//	return true;
		// 	//}

		// 	return false;
		// }

		var_ptr cpp_writer::get_current_container()
		{
			return m_stack.empty() ? nullptr : m_stack.back();
		}

		void cpp_writer::push_current_container(const var_ptr& val)
		{
			m_stack.push_back(val);
		}

		bool cpp_writer::pop_current_container()
		{
			if (m_stack.empty())
				return false;
			m_stack.pop_back();
			return true;
		}

		bool cpp_writer::VisitNull(const vl::Null& var, const char* name)
		{
			// Hold on for a moment. If we pass the whole data to the parent object, then no need to process individual primitive fields.
			// auto val = std::make_shared<primitive_type>(vl::MakePtr(var));
			// return add_member(val, name);
			return true;
		}

		bool cpp_writer::VisitBool(const vl::Bool& var, const char* name)
		{
			// Hold on for a moment. If we pass the whole data to the parent object, then no need to process individual primitive fields.
			// auto val = std::make_shared<primitive_type>(vl::MakePtr(var));
			// return add_member(val, name);
			return true;
		}

		bool cpp_writer::VisitNumber(const vl::Number& var, const char* name)
		{
			// Hold on for a moment. If we pass the whole data to the parent object, then no need to process individual primitive fields.
			// auto val = std::make_shared<primitive_type>(vl::MakePtr(var));
			// return add_member(val, name);
			return true;
		}

		bool cpp_writer::VisitString(const vl::String& var, const char* name)
		{
			// Hold on for a moment. If we pass the whole data to the parent object, then no need to process individual primitive fields.
			// auto val = std::make_shared<primitive_type>(vl::MakePtr(var));
			// return add_member(val, name);
			return true;
		}

		bool cpp_writer::VisitPointer(const vl::Pointer& var, const char* name)
		{
			// Hold on for a moment. If we pass the whole data to the parent object, then no need to process individual primitive fields.
			// auto val = std::make_shared<primitive_type>(vl::MakePtr(var));
			// return add_member(val, name);
			return true;
		}

		structure_ptr cpp_writer::process_root_type(const vl::VarPtr& data, const std::string& var_name)
		{
			auto new_type_holder = m_root_types[var_name] = std::make_shared<value>();
			new_type_holder->init(data, var_name);
			auto new_type = new_type_holder->get_type();
			return new_type->as_ptr<structure>();
		}

		void cpp_writer::process_content(const vl::Object& object)
		{
			begin_writing_fwd();
			object.ForeachProp([self = this](const std::string& field_name, const vl::Var& value) {
				auto type_name = process_new_type_name(field_name);
				if (auto new_type = self->process_root_type(vl::MakePtr(value), field_name))
					new_type->print(type_print_context{ *self, new_type.get() });
				else
				{
					LOCAL_ERROR("The type name '" << type_name << "' is reserved");
					return false;
				}
				return true;
			});
			end_writing_fwd();
		}
		bool cpp_writer::VisitObject(const vl::Object& object, const char* name)
		{
			// Write CPP
			//PRINT_CPP("class " << name);
			//PRINT_CPP("{");
			//m_level++;
			// ==============
			if (!object.IsNull())
			{	// Add a new class if it is not a null object
				if (name)
				{
					if (strcmp(name, "content") == 0)
						process_content(object);
					// auto& data = object;
					// if (std::strcmp(name, "properties") == 0)
					// {
					// 	auto current_container = get_current_container();
					// 	assert(current_container);
					// 	if (!current_container)
					// 	{
					// 		LOCAL_ERROR("Empty container stack when visiting '" << name << "'. You can only define 'properties' inside a type or a field of a type");
					// 		return false;
					// 	}
					// 	if (auto custom_type = current_container->as_ptr<structure>())
					// 	{
					// 		custom_type->set_properties(data);
					// 	}
					// 	else if (auto value = current_container->as_ptr<value>())
					// 	{
					// 		value->set_properties(data);
					// 	}
					// 	else
					// 	{
					// 		LOCAL_ERROR("You can only define 'properties' inside a type or a field of a type");
					// 		return false;
					// 	}
					// }
					// else if (std::strcmp(name, "parameters") == 0)
					// {
					// 	auto current_container = get_current_container();
					// 	assert(current_container);
					// 	if (!current_container)
					// 	{
					// 		LOCAL_ERROR("Empty container stack when visiting '" << name << "'. You can only define 'parameters' inside a custom type");
					// 		return false;
					// 	}
					// 	if (auto custom_type = current_container->as_ptr<structure>())
					// 	{
					// 		custom_type->set_parameters(data);
					// 		return true;
					// 	}
					// 	else
					// 	{
					// 		LOCAL_ERROR("You can only define 'parameters' inside a custom type");
					// 		return false;
					// 	}
					// }
					// else if (std::strcmp(name, "element") == 0)
					// {
					// 	auto current_container = get_current_container();
					// 	assert(current_container);
					// 	if (!current_container)
					// 	{
					// 		LOCAL_ERROR("Empty container stack when visiting '" << name << "'. You can only define 'element' inside a list");
					// 		return false;
					// 	}
					// 	if (auto list = current_container->as_ptr<list>())
					// 	{
					// 		auto value = std::make_shared<list_element>(vl::MakePtr(data), name);
					// 		list->set_element(value);
					// 	}
					// 	else
					// 	{
					// 		LOCAL_ERROR("You can only define 'element' inside a list");
					// 		return false;
					// 	}
					// }
					// else
					// {
					// 	bool is_root = name == nullptr;
					// 	if (is_root)
					// 		begin_writing_fwd();
					// 	if (is_root && m_params.root_name.empty())
					// 		return true; // Skip the root
						
					// 	// TODO: Create root here?

					// 	auto current_container = get_current_container();
					// 	assert(current_container);
					// 	if (current_container->is<structure>())
					// 	{
					// 		auto custom_type = current_container->as_ptr<structure>();
					// 		auto val = std::make_shared<field>(vl::MakePtr(data), process_new_type_name(name));
					// 		if (!add_member(val, name))
					// 			return false;
					// 		push_current_container(val);
					// 		// The type of the field can be created relying on the data passed to the constructor
					// 		// or we need to visit fields one by one
					// 	}
					// 	else
					// 	{
					// 		// This should be a "content" or root, so we just create a custom type
					// 		auto val = std::make_shared<structure>(vl::MakePtr(data), process_new_type_name(name));
					// 		if (!add_member(val, name))
					// 			return false;
					// 	}
					// }
				}
				else
				{
					// Root
				}
			}
			return true;
		}

		bool cpp_writer::EndVisitObject(const vl::ObjectVar& var, const char* name)
		{
			// bool is_root = name == nullptr;
			// if (is_root)
			// 	end_writing_fwd();
			// if (is_root && m_params.root_name.empty())
			// 	return true; // Skip the root
			// auto abstract_container = get_current_container();
			// assert(abstract_container && "A current container should always exist when ending visiting an object");
			// if (!abstract_container)
			// {
			// 	LOCAL_ERROR("Empty container stack when ending visiting object '" << name << "'");
			// 	return false;
			// }
			// assert(abstract_container->is<structure>() && "Ending visiting an object should work with a class container");
			// auto container = abstract_container->as_ptr<structure>();
			// pop_current_container();
			// type_print_context ctx = { *this, *container };
			// if (m_stack.empty()) // is root
			// {
			// 	container->print(ctx);
			// 	m_root = std::make_shared<ui::var>(vl::MakePtr(var), name);
			// }
			// else
			// 	if (auto c = container->as_ptr<structure>())
			// 		c->print(ctx);
				// TODO: other types? (Maybe only custom is enough)
			return true;
		}

		bool cpp_writer::VisitList(const vl::ListVar& var, const char* name)
		{
			// There is no use for JSON arrays at the moment
			//if (!var.IsNull())
			//{
			//	auto val = std::make_shared<list>(vl::MakePtr(vl::List()));
			//	if (!add_member(val, name))
			//		return false;
			//}
			//else
			//{
			//	auto val = std::make_shared<list>(vl::MakePtr(vl::Null()));
			//	if (!add_member(val, name))
			//		return false;
			//}
			return true;
		}

		bool cpp_writer::EndVisitList(const vl::ListVar& var, const char* name)
		{
			pop_current_container();
			return true;
		}

		void cpp_writer::print_fwd(const std::string& type_name)
		{
			INIT_PRINT(m_print_context.fwd_f, 1);
			PRINT_LINE("class " << type_name << ";");
		}

		void cpp_writer::begin_writing_fwd()
		{
			fs::path fpath = m_params.cppgen_params.out_dir_path + "/fwd.h";
			fs::create_directories(fpath.parent_path());
			m_print_context.fwd_f.open(fpath);
			INIT_PRINT(m_print_context.fwd_f, 0);
			PRINT_LINE("#pragma once");
			PRINT_LINE_BREAK;
			PRINT_LINE("namespace " << m_params.cpp_namespace);
			PRINT_SCOPE_BEGIN;
		}

		void cpp_writer::end_writing_fwd()
		{
			m_print_context.fwd_f << "}";
			m_print_context.fwd_f.close();
		}
		// End of cpp_writer
		
		// Begin of var
		bool var::has_proto() const
		{
			if (!m_data)
				return false;
			if (!m_data->is<vl::Object>())
				return false;
			return m_data->as<vl::Object>().GetPrototype();
		}

		bool var::is_reference() const
		{
			if (m_data)
				if (auto data_ptr = m_data->as_ptr<vl::String>())
					return is_path(data_ptr->Val());
			return false;
		}

		std::string var::get_proto_type_name(const vl::TypeResolver& type_resolver) const
		{
			if (!m_data)
				return "";
			if (!m_data->is<vl::Object>())
				return "";
			if (auto proto = m_data->as<vl::Object>().GetPrototype())
				return process_vl_type_name(type_resolver.GetTypeId(proto));
			return "";
		}
		// End of var

		// Begin of value
		// TODO: check if paths are resolved thorugh the vl loader. If not, then implement resolution (mb TypeResolver is enough).
		void value::init(const vl::VarPtr& data, const std::string& value_name, var* parent)
		{
			base::init(data, value_name, parent);
			vl::VarPtr type_data_ptr = nullptr;
			if (data.is<vl::Object>())
			{
				auto& data_obj = data->as<vl::Object>();
				data_obj.ForeachProp([self = this, &data, &value_name](const std::string& field_name, const vl::Var& field_data) {
					if (field_name == "properties")
						self->m_properties = field_data.as<vl::Object>();
					else if (field_name == "base" || !is_reserved_name(field_name))
						if (!self->m_type)
							// TODO: support references to other fields. In this case it should be not a register_custom_type call.
							self->m_type = type_factory::register_custom_type(self);
					return true;
				});
				type_data_ptr = data_obj.Get("type");
			}
			else if (data.is<vl::String>())
			{
				type_data_ptr = data;
			}
			else
			{
				LOCAL_ERROR("Unsupported data type for a value '" << value_name << "'");
				assert(false);
			}

			if (is_reference())
				return;

			if (type_data_ptr)
			{
				auto& type_name = type_data_ptr.as<vl::String>().Val();
				if (!m_type)
				{
					if (auto type = type_factory::get_type(type_name, this))
						m_type = type;
					else
					{
						LOCAL_ERROR("Can't find or create a type '" << type_name << "' for a value '" << value_name << "'");
						assert(false);
					}
				}
				else
				{
					if (!m_type->is<structure>())
					{
						LOG_ERROR("Only custom type can contain another type within. Otherwise, the type should not exist yet and the logic shoud go through the true case of this if.");
						assert(false && "Only custom type can contain another type within. Otherwise, the type should not exist yet and the logic shoud go through the true case of this if.");
					}
				}
			}
		}

		value_const_ptr value::get(const std::string_view& name_or_ref) const
		{
			auto parts = utils::split(name_or_ref, ".");
			if (parts.size() == 1)
			{
				if (auto type = get_type())
					if (auto structure_type = type->as_ptr<structure>())
						if (auto field_ptr_ptr = structure_type->get_field(name_or_ref))
							return std::const_pointer_cast<const value>((*field_ptr_ptr)->as_ptr<value>());
				return nullptr;
			}
			// Iterate every part of the reference
			auto ctx = as_ptr<value>();
			for (const auto& part : parts)
				if (ctx)
					ctx = ctx->get(part);
				else
					break;
			return ctx;
		}
		// End of value

		// Begin of field
		// End of field

		// Begin of type
		// End of type
		
		// Begin of list_element
		void list_element::init(const vl::VarPtr& data, const std::string& name, var* parent)
		{
			base::init(data, name, parent);
			assert(get_type());
		}
		// End of list_element

		// Begin of list
		void list::init(var* var)
		{
			base::init(var->get_name());
			m_var = var;
			auto& data = m_var->get_data();
			assert(data.is<vl::Object>());
			auto& data_obj = data->as<vl::Object>();
			auto field_name = "element";
			assert(data_obj.Has(field_name));
			auto& element_data_ptr = data_obj.Get(field_name);
			m_element = std::make_shared<list_element>();
			m_element->init(element_data_ptr, field_name, m_var);
		}

		int list::print(type_print_context& ctx) const
		{
			auto& name = get_name();
			if (!ctx.file)
			{
				LOCAL_ERROR("list::print() has been called with no file provided in the context");
				return -1;
			}

			std::ofstream& f = *ctx.file;
			INIT_PRINT(f, ctx.indent_level);
			// TODO: support references to other fields. In this case, the element type should be printed.
			// TODO: support system types if it makes sense.
			auto element_type = m_element->get_type()->as_ptr<structure>();
			assert(element_type);
			element_type->print(ctx);
			PRINT_LINE("using " << name << "_t = utils::ui::structure::vector<" << element_type->get_name() << ">;")
		}
		// End of list
		// Begin of structure
		void structure::init(const std::string& type_name, var* var)
		{
			base::init(type_name);
			m_var = var;
			auto& data = m_var->get_data().as<vl::Object>();
			vl::VarPtr root_data_ptr;
			if (data.Has("type"))
			{
				// Transform a system typed field to a custom type by adding a root node with the system type specified in the "type" field
				root_data_ptr = vl::Object();
				root_data_ptr["type"] = data["type"];
			}
			data.ForeachProp([self = this, &root_data_ptr](const std::string& field_name, const vl::Var& field_value) {
				if (is_reserved_name(field_name))
				{
					if (field_name == "base")
					{
						const std::string& base_type_name = field_value.as<vl::String>().Val();
						if (auto base_type = type_factory::find_type(base_type_name))
							self->m_base_type = base_type;
						else
							LOCAL_ERROR("Can't find a base type '" << base_type_name << "' for a custom type '" << self->get_name() << "'");
					}
					else
					{
						if (root_data_ptr)
							root_data_ptr[field_name] = field_value;
						if (field_name == "properties")
							self->m_properties = field_value.as<vl::Object>();
						else if (field_name == "parameters")
							self->m_parameters = field_value.as<vl::Object>();
						}
				}
				else
				{
					auto field = std::make_shared<class field>();
					field->init(vl::MakePtr(field_value), field_name);
					self->add_field(field_name, field);
				}

				return true;
			});
			if (root_data_ptr)
			{
				auto field = std::make_shared<class field>();
				field->init(root_data_ptr, "root");
				add_field("root", field);
			}
		}

		int structure::print(type_print_context& ctx) const
		{
			std::ofstream local_file;
			std::ofstream& f = ctx.file ? *ctx.file : local_file;
			auto& name = get_name();
			bool has_own_file = true;
			if (!ctx.file && !has_own_file)
			{
				LOCAL_ERROR("Can't provide an output file for class '" << name << "' as it is not a type and there is no file provided in the print context");
				return -1;
			}

			// Open the file
			if (has_own_file)
			{
				auto& wctx = ctx.writer.print_context();
				if (wctx.printed_classes.find(name) != wctx.printed_classes.end())
					return 0; // Already printed or printing
				else
				{
					wctx.printed_classes.emplace(name);
					ctx.writer.print_fwd(name);
				}
				fs::path fpath = ctx.writer.get_params().cppgen_params.out_dir_path + "/" + name + ".h";
				fs::create_directories(fpath.parent_path());
				f.open(fpath);
				if (!f.is_open())
				{
					LOCAL_ERROR("Can't write to a file '" << fpath << "'");
					return -2;
				}
			}

			print_data pdata;
			int collect_data_result = collect_data(pdata, ctx);
			if (collect_data_result < 0)
				return collect_data_result;
			
			auto& proto_id = pdata.proto_id;

			INIT_PRINT(f, ctx.indent_level);

			// === Start writing ===
			// Predefines section
			auto& subclasses = pdata.subclasses;
			//auto& primitives = pdata.primitives;
			auto& generated_types_to_include = pdata.generated_types_to_include;
			auto& includes = pdata.includes;
			auto& fields = pdata.fields;
			
			if (has_own_file)
			{
				PRINT_LINE("#pragma once");
				PRINT_LINE_BREAK;

				// Includes
				PRINT_LINE("#include <abstract_ui/fwd.h>");
				if (!generated_types_to_include.empty())
				{
					PRINT_LINE_BREAK;
					for (auto& inc : generated_types_to_include)
						PRINT_LINE("#include \"" << inc << ".h\"");
				}
				if (!includes.empty())
				{
					PRINT_LINE_BREAK;
					for (auto& inc : includes)
						PRINT_LINE("#include " << inc);
				}
				
				PRINT_LINE_BREAK;

				// Write namespace
				PRINT_LINE("namespace " << ctx.writer.get_params().cpp_namespace);
				PRINT_LINE("{");
				PRINT_INDENT_INCREASE;
			}
			// Print class declaration header
			BEGIN_PRINT_LINE;
			PRINT(f, "class " << get_name());
			std::vector<std::string> base_classes;
			if (m_base_type)
				base_classes.push_back(m_base_type->get_name());
			if (!proto_id.empty())
				base_classes.push_back(proto_id);
			
			for (size_t i = 0; i < base_classes.size(); i++)
			{
				if (i == 0)
					PRINT(f, " : ");
				else
					PRINT(f, ", ");
				PRINT(f, "public " << base_classes[i]);
			}
			END_PRINT_LINE;

			PRINT_LINE("{");
			PRINT_LINE("public:");
			PRINT_INDENT_INCREASE;
			
			// Declare init method
			PRINT_LINE(INIT_METHOD_DECLARATION << ";");

			if (!m_fields.empty())
			{
				// Print nested classes
				if (!pdata.custom_types.empty())
				{
					PRINT_LINE("// Nested object fields classes")
					PRINT_INDENT_DECREASE;
					type_print_context subctx = { ctx.writer, this , ctx.file, ctx.indent_level };
					subctx.file = &f;
					subctx.indent_level = INDENT_LEVEL + 1;
					subctx.parent_context = &ctx;
					for (auto&& t : pdata.custom_types)
						t->print(subctx);
					PRINT_INDENT_INCREASE;
				}
				
				PRINT_LINE("// Field access");
				
				foreach_field(*this, [&](const std::string& n, const var_ptr& f, int recursion_level) {
					PRINT_LINE("// \"" << n << "\" field");
					if (auto c = f->as_ptr<structure>())
					{
						PRINT_LINE(CLASS_GETTER_DECLARATION("", "get_" << n, c, ""));
						PRINT_LINE(CLASS_GETTER_DECLARATION("const ", "get_" << n, c, " const"));
					}
					else if (auto l = f->as_ptr<list>())
					{
						PRINT_LINE(MEHTOD_DECLARATION("vl::List&", n, "", ""));
						PRINT_LINE(MEHTOD_DECLARATION("const vl::List&", "get_" + n, "", " const"));
					}
					// It is not clear what to do with primitives as it may be enough to parse vl data by a need
					//else if (auto p = f->as_primitive_type())
					//{
					//	if (p->is_bool()) {
					//		PRINT_LINE(MEHTOD_DECLARATION("bool", n, "", " const"));
					//		if (ctx.writer.get_params().cppgen_params.generate_setters)
					//			PRINT_LINE(MEHTOD_DECLARATION("void", "set_" << n, "bool value", ""));
					//	} else if (p->is_number()) {
					//		PRINT_LINE(MEHTOD_DECLARATION("float", n, "", " const"));
					//		if (ctx.writer.get_params().cppgen_params.generate_setters)
					//			PRINT_LINE(MEHTOD_DECLARATION("void", "set_" << n, "float value", ""));
					//	} else if (p->is_string()) {
					//		PRINT_LINE(MEHTOD_DECLARATION("const std::string&", n, "", " const"));
					//		if (ctx.writer.get_params().cppgen_params.generate_setters)
					//			PRINT_LINE(MEHTOD_DECLARATION("void", "set_" << n, "const std::string& value", ""));
					//	} else if (p->is_pointer()) {
					//		PRINT_LINE(MEHTOD_DECLARATION("void*", n, "", " const"));
					//		if (ctx.writer.get_params().cppgen_params.generate_setters)
					//			PRINT_LINE(MEHTOD_DECLARATION("void", "set_" << n, "const void* value", ""));
					//	}
					//}
					PRINT_LINE_BREAK;
				});
				
				PRINT_INDENT_DECREASE;
			}
			else
				PRINT_INDENT_DECREASE;
			// Print members
			
			if (pdata.proto_id.empty())
			{
				// Declare _data_() method
				PRINT_LINE("protected:");
				PRINT_INDENT_INCREASE;
				PRINT_LINE("// Data getter for internal use");
				PRINT_LINE("const vl::VarPtr& _data_() const {");
				PRINT_INDENT_INCREASE;
				PRINT_LINE("return m_data;");
				PRINT_SCOPE_END;
				PRINT_INDENT_DECREASE;
				PRINT_LINE_BREAK;
			}
			if (!fields.empty() || !m_var->has_proto())
			{
				PRINT_LINE("private:");
				PRINT_INDENT_INCREASE;
				PRINT_LINE("// Data members");
				for(auto&& f : fields)
				{
					auto& fn = f->get_name();
					auto type = f->get_type();
					assert(type);
					if (type->is<structure>())
						PRINT_LINE("class " << type->get_name() << " " << fn << ";")
					else if (type->is<system_type>())
						PRINT_LINE(type->get_name() << "_ptr " << fn << ";")
				};
				PRINT_INDENT_DECREASE;
			}
			// Close class declaration
			PRINT_LINE("};");
			if (has_own_file)
			{
				// Close namespace
				PRINT_SCOPE_END;
				
				// Print .cpp
				std::ofstream fcpp;
				auto& name = get_name();
				fs::path fcpp_path = ctx.writer.get_params().cppgen_params.out_dir_path + "/" + name + ".cpp";
				fs::create_directories(fcpp_path.parent_path());
				fcpp.open(fcpp_path);
				if (!fcpp.is_open())
				{
					LOCAL_ERROR("Can't write to a file '" << fcpp_path << "'");
					return -2;
				}
				
				INIT_PRINT(fcpp, 0);
				// Includes
				// Include VL
				PRINT_LINE("#include \"" << get_name() << ".h\"");
				PRINT_LINE_BREAK;
				PRINT_LINE("namespace " << ctx.writer.get_params().cpp_namespace);
				PRINT_SCOPE_BEGIN;
				type_print_context defs_ctx = { ctx.writer, this, &fcpp, ctx.indent_level };
				defs_ctx.indent_level = INDENT_LEVEL;
				print_definitions_context print_defs_ctx = {defs_ctx, &pdata};
				print_definitions(print_defs_ctx);
				PRINT_SCOPE_END;
			}
			return 0;
		}

		void structure::print_definitions(print_definitions_context& pctx) const
		{
			auto& ctx = pctx.ctx;
			print_data pdata;
			if (!pctx.pdata)
				collect_data(pdata, ctx);
			auto& subclasses = pctx.pdata ? pctx.pdata->subclasses : pdata.subclasses;
			//auto& primitives = pctx.pdata ? pctx.pdata->primitives : pdata.primitives;
			// Define constructors
			INIT_PRINT(*pctx.ctx.file, pctx.ctx.indent_level);
			
			auto print_constructor_body = [&]() {
				PRINT_SCOPE_BEGIN;
				PRINT_LINE("init(owner);");
				PRINT_SCOPE_END;
			};

			auto print_init_method_body = [&]() {
				PRINT_SCOPE_BEGIN;
				auto base_type = get_base_type();
				// Check m_data
				if (!subclasses.empty())
				{
					// Print class members initialisers
					for (auto&& f : pdata.fields) {
						auto& fn = f->get_name();
						auto type = f->get_type();
						assert(type);
						if (type->is<system_type>())
						{
							auto& tn = type->get_name();
							PRINT_LINE("" << fn << " = owner->create<" << tn << ">();");
						}
						else if (auto s = type->as_ptr<structure>())
						{
							PRINT_LINE("" << fn << ".init(owner);");
						}
					}
				}
				PRINT_SCOPE_END;
			};
			
			PRINT_LINE("// Initializers");
			
			PRINT_LINE_BREAK;
			
			PRINT_LINE(INIT_METHOD_SIGNATURE(CLASS_CPP_SCOPE));
			print_init_method_body();
			PRINT_LINE_BREAK;

			// Getters and setters definitions
			if (!m_fields.empty())
			{
				PRINT_LINE("// Field access");
				// fn - field name
				foreach_field(*this, [&](const std::string& fn, const var_ptr& v, int recursion_level) {
					// We don't make getters for the fields as we have them as class members
				});
			}
			
			// Subclasses defenitions
			if (!subclasses.empty())
			{
				PRINT_LINE("// Subclasses definitions begin")
				for (auto&& f : subclasses)
				{
					auto& n = f.first;
					auto& c = f.second;
					type_print_context subctx = { ctx.writer, c->as_ptr<structure>().get() , ctx.file, INDENT_LEVEL, &ctx};
					print_definitions_context print_defs_subctx = { subctx };
					c->as_ptr<structure>()->print_definitions(print_defs_subctx);
				}
			}
		}

		// TODO: mb move it to var?
		int structure::collect_data(print_data& pdata, type_print_context& ctx) const
		{
			auto& data = m_var->get_data();
			if (!data)
			{
				LOCAL_ERROR("Attemption to print an unitialized class '" << get_name() << "'");
				return -3;
			}

			if (!data->is<vl::Object>())
			{
				LOCAL_ERROR("Attemption to print a wrongly itialized class '" << get_name() << "'. Data should be of 'Object' type");
				return -4;
			}
			
			auto& data_obj = data->as<vl::Object>();
			
			// Get proto id
			if (auto proto = data_obj.GetPrototype())
				pdata.proto_id = process_vl_type_name(ctx.writer.get_type_resolver().GetTypeId(proto));
			
			if (!pdata.proto_id.empty())
				if (!is_path(pdata.proto_id))
					pdata.generated_types_to_include.emplace(pdata.proto_id);

			foreach_field(*this, [&](const std::string& __n, const field_ptr& __f, int) {
				__f->collect_data_recursion(ctx, pdata, 0);
			}, 0);
			return 0;
		}

		void structure::field::collect_data_recursion(
			type_print_context& ctx
			, structure::print_data& pdata
			, int recursion_level
		)
		{
			// Works recursively
			// Works only on the first recursion level
			if (recursion_level == 0)
				pdata.fields.push_back(as_ptr<structure::field>());
			auto type = get_type();
			assert(type);
			
			// Ignore primiteves for this time
			//else if (auto p = f->as_primitive_type())
			//{
			//	pdata.primitives.add(n, f);
			//}
			if (auto c = type->as_ptr<structure>())
			{
				if (recursion_level == 0)
				{
					pdata.subclasses.add(get_name(), c);
					pdata.custom_types.push_back(c);
				}

				if (c->m_var->has_proto())
				{
					auto include = c->m_var->get_proto_type_name(ctx.writer.get_type_resolver());
					if (!is_path(include))
						pdata.generated_types_to_include.emplace(include);
				}
				if (auto base_type = c->get_base_type())
				{
					auto include = base_type->get_include_path();
					pdata.includes.emplace(include);
				}
			}
			else if (auto c = type->as_ptr<system_type>())
			{
				pdata.includes.emplace(c->get_include_path());
			}
			else if (auto l = type->as_ptr<list>())
			{
				if (recursion_level == 0)
					pdata.custom_types.push_back(l);
				pdata.includes.emplace("<abstract_ui/structure/list.h>");
			}
		}

		// End of structure
	}
}
