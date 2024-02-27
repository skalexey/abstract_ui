#pragma once

#include <abstract_ui/widgets/menu.h>
#include <abstract_ui/qt/widget.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class menu : public qt::widget, public virtual ui::menu
			{
			public:
				using base = ui::menu;

			protected:
				int init() override;
			};
			using menu_ptr = std::shared_ptr<menu>;
		}
	}
}