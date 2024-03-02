#pragma once

#include <abstract_ui/widgets/menu_list.h>
#include <abstract_ui/qt/widget.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class menu_list : public qt::widget, public virtual ui::menu_list
			{
			public:
				using base = ui::menu_list;
			
				void add_widget(const ui::widget_ptr& widget) override;
				
			protected:
				void on_set_title() override;
				int init() override;
				widget_model* create_model() const override;
			};
			using menu_list_ptr = std::shared_ptr<menu_list>;
		}
	}
}