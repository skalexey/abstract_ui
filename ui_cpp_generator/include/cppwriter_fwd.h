#pragma once
#include <memory>

namespace utils
{
	namespace ui
	{
		class cpp_writer;
		struct type_print_context;
		class var;
		using var_ptr = std::shared_ptr<var>;
		using var_const_ptr = std::shared_ptr<const var>;
		class type;
		using type_ptr = std::shared_ptr<type>;
		class class_desc;
		using class_desc_ptr = std::shared_ptr<class_desc>;
		class custom_type;
		using custom_type_ptr = std::shared_ptr<custom_type>;
		class structure;
		using structure_ptr = std::shared_ptr<structure>;
		class system_type;
		class embedded_type;
		class list_element;
		using list_element_ptr = std::shared_ptr<list_element>;
		class list;
		using list_ptr = std::shared_ptr<list>;
		class primitive_type;
		using primitive_type_ptr = std::shared_ptr<primitive_type>;
		class value;
		using value_ptr = std::shared_ptr<value>;
		using value_const_ptr = std::shared_ptr<const value>;
	}
}