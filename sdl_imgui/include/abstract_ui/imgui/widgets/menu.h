#pragma once

#include <abstract_ui/widgets/menu.h>
#include <abstract_ui/imgui/widget.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class menu : public imgui::widget, public virtual ui::menu
			{
			public:
				using base = ui::menu;
				
			protected:
				bool update_children(float dt) override final;
				
			};
			using menu_ptr = std::shared_ptr<menu>;
		}
	}
}