#pragma once

#include <abstract_ui/imgui/widget.h>
#include <abstract_ui/widgets/layouts/horizontal_layout.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class horizontal_layout : public ui::horizontal_layout, public virtual imgui::widget
			{
			public:
				using base = ui::horizontal_layout;

			protected:
				bool update_child(ui::node* const child, float dt) override;
			};
		}
	}
}