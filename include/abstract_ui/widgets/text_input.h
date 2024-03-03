#pragma once

#include <memory>
#include <string>
#include <abstract_ui/widgets/value_input.h>

namespace utils
{
	namespace ui
	{
		class text_input : public value_input<std::string>
		{
		public:
			using base = value_input<std::string>;
		};
		//using text_input = value_input<std::string>;
		using text_input_ptr = std::shared_ptr<text_input>;
	}
}