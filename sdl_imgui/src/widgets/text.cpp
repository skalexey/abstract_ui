#pragma once

#include <imgui.h>
#include <abstract_ui/imgui/widget_factory.h>
#include <abstract_ui/imgui/widgets/text.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			REGISTER_WIDGET(text);

			bool imgui::text::on_update(float dt)
			{
				if (!imgui_text_update(dt))
					return false;
				return imgui::widget::on_update(dt);
			}

			bool imgui::text::imgui_text_update(float dt)
			{
				ImGui::TextWrapped("%s", get_text().c_str());
				return true;
			}
		}
	}
}