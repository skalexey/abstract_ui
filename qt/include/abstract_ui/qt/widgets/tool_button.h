#pragma once

#include <abstract_ui/widgets/menu_item.h>
#include <abstract_ui/qt/widgets/clickable_item.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class tool_button : public qt::clickable_item, public virtual ui::menu_item
			{
			public:
				using base = ui::menu_item;
				
			protected:
				int init() override;
			};
			using tool_button_ptr = std::shared_ptr<tool_button>;
		}
	}
}