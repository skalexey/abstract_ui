#pragma once

#include <abstract_ui/widgets/menu_item.h>
#include <abstract_ui/qt/widgets/clickable_item.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class menu_item : public qt::clickable_item, public virtual ui::menu_item
			{
			public:
				using base = ui::menu_item;
				
			protected:
				int init() override;
			};
			using menu_item_ptr = std::shared_ptr<menu_item>;
		}
	}
}