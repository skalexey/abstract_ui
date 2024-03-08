#include <utils/string_utils.h>
#include <abstract_ui/imgui/widgets/menu_item.h>
#include <imgui.h>
#include <utils/log.h>
LOG_TITLE("imgui::menu_item");

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			bool imgui::menu_item::on_update(float dt)
			{
				if (!imgui_menu_item_update(dt))
					return false;
				return imgui::widget::on_update(dt);
			}

			bool imgui::menu_item::imgui_menu_item_update(float dt)
			{
				if (ImGui::MenuItem(get_text().c_str(), NULL, nullptr, m_enabled))
					on_click(true);
				return true;
			}
		}
	}
}