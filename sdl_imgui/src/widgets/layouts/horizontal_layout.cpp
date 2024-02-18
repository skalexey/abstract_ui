#include <imgui.h>
#include <abstract_ui/imgui/widgets/layouts/horizontal_layout.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			bool imgui::horizontal_layout::update_child(ui::node* const child, float dt)
			{
				auto& children = get_children();
				if (child != children.front().get())
					ImGui::SameLine();
				return base::update_child(child, dt);
			}
		}
	}
}