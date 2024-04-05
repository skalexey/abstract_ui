#pragma once

#include "cpp_writer.h"

namespace utils
{
	namespace ui
	{
		class list_var : public var
		{
			using base = var;

		public:
			using fields_list = std::vector<var_ptr>;

			list(const vl::VarPtr& data, const std::string& name)
				: base(data, name)
			{}
			bool is_empty() {
				return m_fields.empty();
			}
			const var_desc_ptr& add(const var_desc_ptr& field) {
				m_fields.push_back(field);
				return m_fields.back();
			}
			const fields_list& get_fields() const {
				return m_fields;
			}

		private:
			fields_list m_fields;
		};

		// Not used at the moment, but may be useful in the future for more powerful configuration
		class primitive_type : public type
		{
		public:
			using base = type;

			primitive_type(const vl::VarPtr& data, const std::string& name)
				: base(data, name)
			{}
			bool is_bool() const {
				return get_data() && get_data().is<vl::Bool>();
			};
			bool is_number() const {
				return get_data() && get_data().is<vl::Number>();
			};
			bool is_string() const {
				return get_data() && get_data().is<vl::String>();
			};
		};
		using primitive_type_ptr = std::shared_ptr<primitive_type>;
	}
}