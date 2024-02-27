#pragma once

#include <abstract_ui/widgets/menu_list.h>
#include <abstract_ui/imgui/widget.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class menu_list : public imgui::widget, public virtual ui::menu_list
			{
			public:
				using base = ui::menu_list;
				
			protected:
				bool update_children(float dt) override final;

			private:
				bool update_menu_list(float dt);
			};
			using menu_list_ptr = std::shared_ptr<menu_list>;
		}
	}
}