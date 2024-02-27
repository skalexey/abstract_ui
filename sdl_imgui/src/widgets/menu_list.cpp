#include <utils/string_utils.h>
#include <abstract_ui/imgui/widgets/menu_list.h>
#include <imgui.h>
#include <utils/Log.h>
LOG_TITLE("imgui::menu_list");

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			bool imgui::menu_list::update_children(float dt)
			{
				bool result = true;
				if (ImGui::BeginMenu(get_title().c_str()))
				{
					result = base::update_children(dt);
					ImGui::EndMenu();
				}
				return result;
			}
		}
	}
}