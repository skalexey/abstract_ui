#include <utils/string_utils.h>
#include <abstract_ui/imgui/widgets/dialog.h>
#include <imgui.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			imgui::dialog::dialog()
			{
				set_modal(false); // Modal dialogs in imgui:: are not supported yet
				// as they are buggy if showing more than one at a time
				set_title("ImGui Dialog");
			}

			bool imgui::dialog::update_children(float dt)
			{
				if (!is_open())
					return false;

				auto font = ImGui::GetFont();
				font->Scale = 1.5f;
				// Set window position
				auto& pos = get_position();
				auto& sz = get_size();

				ImGui::SetNextWindowPos(
					ImVec2(float(pos.x), float(pos.y))
					, pos == m_last_position ? ImGuiCond_Appearing : ImGuiCond_Always
				);
				m_last_position = pos;
				ImGui::SetNextWindowSize(
					ImVec2(float(sz.x), float(sz.y))
					, sz == m_last_size ? ImGuiCond_Appearing : ImGuiCond_Always
				);
				m_last_size = sz;
				// Create the window

				ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
				
				if (is_auto_resize())
					window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

				auto label = utils::format_str("%s##", get_title().c_str());
				bool p_open = true;
				auto p_open_ptr = is_close_button_enabled() ? &p_open : nullptr;
				
				if (is_modal())
				{
					ImGui::OpenPopup(label.c_str());
					if (!ImGui::BeginPopupModal(label.c_str(), p_open_ptr, window_flags))
					{
						// Early out if the window is collapsed, as an optimization.
						ImGui::EndPopup();
						return true;
					}
				}
				else
				{
					if (!ImGui::Begin(label.c_str(), p_open_ptr, window_flags))
					{
						// Early out if the window is collapsed, as an optimization.
						ImGui::End();
						return true;
					}
				}

				if (!base::update_children(dt))
					return false;

				// Close button
				if (!p_open)
					close();

				// End the window scope
				if (is_modal())
					ImGui::EndPopup();
				else
					ImGui::End();
				return true;
			}
		}
	}
}