#pragma once

#include <memory>
#include <abstract_ui/widgets/menu_list_base.h>

namespace utils
{
	namespace ui
	{
		class menu_bar : public virtual menu_list_base
		{
		public:
			using base = menu_list_base;
		};
		using menu_bar_ptr = std::shared_ptr<menu_bar>;
	}
}