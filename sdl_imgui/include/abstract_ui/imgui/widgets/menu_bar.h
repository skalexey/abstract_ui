#pragma once

#include <abstract_ui/widgets/menu_bar.h>
#include <abstract_ui/imgui/widget.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class menu_bar : public virtual imgui::widget, public virtual ui::menu_bar
			{
			public:
				using base = ui::menu_bar;
				
			protected:
				bool update_children(float dt) override final;
				
			};
			using menu_bar_ptr = std::shared_ptr<menu_bar>;
		}
	}
}