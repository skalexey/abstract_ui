#pragma once

#include <string>
#include "cppgen_fwd.h"

namespace utils
{
	namespace ui
	{
		class cpp_generator
		{
		public:
			int generate(const cpp_generator_params& __params);
		};
	}
}