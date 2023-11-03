#pragma once

#include <abstract_ui/fwd.h>
#include <abstract_ui/node.h>

namespace utils
{
	namespace ui
	{
		template <typename T>
		class final : public T
		{
			
		};
	}
}
