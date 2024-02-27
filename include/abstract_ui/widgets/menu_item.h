#pragma once

#include <memory>
#include <abstract_ui/widgets/clickable_item.h>

namespace utils
{
	namespace ui
	{
		class menu_item : public virtual clickable_item
		{
		public:
			using base = clickable_item;
		};
		using menu_item_ptr = std::shared_ptr<menu_item>;
	}
}