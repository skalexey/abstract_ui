#include <cmath>
#include <utils/string_utils.h>
#include <abstract_ui/imgui/widgets/menu_list.h>
#include <abstract_ui/imgui/widgets/menu_bar.h>
#include <imgui.h>
#include <utils/log.h>
LOG_TITLE("imgui::menu_list");

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			bool imgui::menu_list::update_menu_list(float dt)
			{
				bool result = true;
				if (ImGui::BeginMenu(get_title().c_str()))
				{
					result = base::update_children(dt);
					ImGui::EndMenu();
				}
				return result;
			}

			bool imgui::menu_list::update_children(float dt)
			{
				bool result = true;
				auto parent = get_parent();
				if (parent && parent->is<menu_bar>() || parent->is<menu_list>())
				{
					return update_menu_list(dt);
				}
				else // Draw the menu list in a separate window
				{
					auto& sz = get_size();
					// Set the size of the menu
					auto text_size = ImGui::CalcTextSize(get_title().c_str());
					auto font = ImGui::GetFont();
					text_size.x *= font->Scale;
					text_size.y *= font->Scale;
					auto window_size = ImVec2(std::fmax(text_size.x, std::ceil(text_size.x)), std::fmax(text_size.y, std::ceil(text_size.y)));
					ImGui::SetNextWindowSize(
						window_size
						, ImGuiCond_Always
					);
					// Set the position of the menu
					auto& pos = get_position();
					ImGui::SetNextWindowPos(
						ImVec2(pos.x, pos.y)
						, ImGuiCond_Always
					);
					auto window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;// | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;
					if (ImGui::Begin(utils::format_str("##menu_list_window_%s", get_title().c_str()).c_str(), nullptr, window_flags))
					{
						result = update_menu_list(dt);
					}
					ImGui::End();
				}
				return result;
			}
		}
	}
}