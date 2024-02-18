#pragma once

#include <abstract_ui/imgui/widget.h>
#include <abstract_ui/widgets/layouts/vertical_layout.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class vertical_layout : public ui::vertical_layout, public virtual imgui::widget
			{
			public:
				using base = ui::vertical_layout;
			};
		}
	}
}