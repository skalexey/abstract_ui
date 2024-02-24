#include <utils/string_utils.h>
#include <abstract_ui/imgui/widgets/menu.h>
#include <imgui.h>
#include <utils/Log.h>
LOG_TITLE("imgui::menu");

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			bool imgui::menu::update_children(float dt)
			{
				if (!is_open())
					return false;

				auto font = ImGui::GetFont();
				font->Scale = 1.5f;

				// Set window position and size
				ImGui::SetNextWindowPos(
					ImVec2(0, 0)
					, ImGuiCond_Always
				);
				auto screen_size = get_screen_size();
				ImGui::SetNextWindowSize(
					ImVec2(screen_size.x, screen_size.y)
					, ImGuiCond_Always
				);

				// Create the window
				ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
				
				auto label = utils::format_str("%s##", get_title().c_str());
				bool p_open = true;
				auto p_open_ptr = is_back_button_enabled() ? &p_open : nullptr;
				
				if (!ImGui::Begin(label.c_str(), p_open_ptr, window_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return true;
				}

				if (!base::update_children(dt))
					return false;

				// back button
				if (!p_open)
					close();

				// End the window scope
				ImGui::End();
				return true;
			}
		}
	}
}