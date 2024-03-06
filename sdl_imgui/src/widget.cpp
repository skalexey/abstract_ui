#pragma once

#include <imgui.h>
#include <utils/string_utils.h>
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

			bool imgui::widget::update_children(float dt)
			{
				return base::update_children(dt);
				// TODO: implement basic implementation for an empty widget that applies its properties to its children like this:
				// bool result = true;
				// auto parent = get_parent();
				// auto& sz = get_size();
				// // Set the size of the menu
				// auto text_size = ImGui::CalcTextSize(get_title().c_str());
				// auto font = ImGui::GetFont();
				// text_size.x *= font->Scale;
				// text_size.y *= font->Scale;
				// auto window_size = ImVec2(std::fmax(text_size.x, std::ceil(text_size.x)), std::fmax(text_size.y, std::ceil(text_size.y)));
				// ImGui::SetNextWindowSize(
				// 	window_size
				// 	, ImGuiCond_Always
				// );
				// // Set the position of the menu
				// auto& pos = get_position();
				// ImGui::SetNextWindowPos(
				// 	ImVec2(pos.x, pos.y)
				// 	, ImGuiCond_Always
				// );
				// auto window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
				// if (ImGui::Begin(utils::format_str("##widget_window_%s", get_title().c_str()).c_str(), nullptr, window_flags))
				// {
				// 	result = update_widget(dt);
				// }
				// ImGui::End();
				// return result;
			}
		}
	}
}