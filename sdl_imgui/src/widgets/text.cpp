#pragma once

#include <imgui.h>
#include <abstract_ui/imgui/widget_factory.h>
#include <abstract_ui/imgui/widgets/text.h>

namespace
{
	REGISTER_WIDGET_DECLARATIVELY(utils::ui::imgui::widget_factory, utils::ui::imgui::text);
}

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			bool imgui::text::imgui_text_update(float dt)
			{
				ImGui::TextWrapped("%s", get_text().c_str());
				return true;
			}
		}
	}
}