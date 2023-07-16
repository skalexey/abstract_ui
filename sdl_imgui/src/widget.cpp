#pragma once

#include <imgui.h>
#include <abstract_ui/widget.h>
#include <abstract_ui/imgui/widget.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			vec2i imgui::widget::get_screen_size() const
			{
				auto& s = ImGui::GetIO().DisplaySize;
				return vec2i(s.x, s.y);
			}
		}
	}
}