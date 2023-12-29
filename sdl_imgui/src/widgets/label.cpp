#pragma once

#include <abstract_ui/imgui/widget_factory.h>
#include <abstract_ui/imgui/widgets/label.h>
#include <imgui.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			REGISTER_WIDGET_DECLARATIVELY(imgui::widget_factory, label);

			bool imgui::label::on_update(float dt)
			{
				ImGui::Text("%s", get_text().c_str());
				if (!on_imgui_label_update(dt))
					return false;
				if (!imgui::widget::on_update(dt))
					return false;
				if (!ui::label::on_update(dt))
					return false;
				return true;
			}
		}
	}
}