#pragma once

#include <utils/string_utils.h>
#include <abstract_ui/widgets/menu_bar.h>
#include <abstract_ui/imgui/window.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			bool imgui::window::update_children(float dt)
			{
				if (!is_open())
					return false;

				auto font = ImGui::GetFont();
				font->Scale = 1.5f;

				// Set window position
				auto& pos = get_position();
				ImGui::SetNextWindowPos(
					ImVec2(pos.x, pos.y)
					, ImGuiCond_Always
				);
				auto& sz = get_size();
				ImGui::SetNextWindowSize(
					ImVec2(sz.x, sz.y)
					, ImGuiCond_Always
				);

				auto p_open = this->p_open();
				// Create the window
				auto label = utils::format_str("%s##", get_title().c_str());
				if (!ImGui::Begin(label.c_str(), p_open, window_flags()))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return true;
				}
					
				if (!base::update_children(dt))
					return false;

				// Close button
				if (p_open && !*p_open)
					close();
					
				// End the window scope
				ImGui::End();
				return true;
			}

			void imgui::window::on_add_node(ui::node* node)
			{
				if (node->is<menu_bar>())
					m_window_flags |= ImGuiWindowFlags_MenuBar;
			}
		}
	}
}