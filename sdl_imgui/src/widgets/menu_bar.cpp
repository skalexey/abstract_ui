#include <utils/string_utils.h>
#include <imgui.h>
#include <utils/log.h>
#include <abstract_ui/imgui/widgets/menu_bar.h>
LOG_TITLE("imgui::menu_bar");

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			bool imgui::menu_bar::update_children(float dt)
			{
				bool result = true;
				if (ImGui::BeginMenuBar())
				{
					result = base::update_children(dt);
					ImGui::EndMenuBar();
				}
				return result;
			}
		}
	}
}