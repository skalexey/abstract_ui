#include <abstract_ui/imgui/widget_factory.h>
#include <abstract_ui/imgui/widgets/dialogs/dialog_message.h>
#include <abstract_ui/imgui/common.h>
#include <imgui.h>

namespace
{
	REGISTER_WIDGET_DECLARATIVELY(utils::ui::imgui::widget_factory, utils::ui::imgui::dialog_message);
}

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			imgui::dialog_message::dialog_message()
			{
				do_on_post_construct([self = this] {
					self->ok_button().set_on_before_update([self](float dt) {
						ImGuiStyle& style = ImGui::GetStyle();
						float width = 0.0f;
						width += self->ok_button().get_size().x;
						utils::ui::imgui::AlignForWidth(width);
						return true;
					});
					return 0;
				});
			}
		}
	}
}