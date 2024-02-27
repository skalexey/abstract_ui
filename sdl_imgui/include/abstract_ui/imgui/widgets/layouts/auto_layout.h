#pragma once

#include <abstract_ui/imgui/widget.h>
#include <abstract_ui/widgets/layouts/auto_layout.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class auto_layout : public ui::auto_layout, public virtual imgui::widget
			{
			public:
				using base = ui::auto_layout;
			};
		}
	}
}