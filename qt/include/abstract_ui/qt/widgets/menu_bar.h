#pragma once

#include <abstract_ui/widgets/menu_bar.h>
#include <abstract_ui/qt/widget.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class menu_bar : public qt::widget, public virtual ui::menu_bar
			{
			public:
				using base = ui::menu_bar;
				// void add_widget(const ui::widget_ptr& widget) override;
				// void add_item(const std::string& title, const action_t& action) override;
			protected:
				int init() override;
			};
			using menu_bar_ptr = std::shared_ptr<menu_bar>;
		}
	}
}