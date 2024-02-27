#include <abstract_ui/imgui/widget_factory.h>
#include <abstract_ui/imgui/widgets/dialogs/dialog_yes_no.h>
#include <abstract_ui/imgui/common.h>
#include <imgui.h>

namespace
{
	REGISTER_WIDGET_DECLARATIVELY(utils::ui::imgui::widget_factory, utils::ui::imgui::dialog_yes_no);
}

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			REGISTER_WIDGET_DECLARATIVELY(imgui::widget_factory, imgui::dialog_yes_no);

			int imgui::dialog_yes_no::on_dialog_yes_no_post_construct()
			{
				yes_button().set_on_before_update([this](float dt) {
					ImGuiStyle& style = ImGui::GetStyle();
					float width = 0.0f;
					width += yes_button().get_size().x;
					width += style.ItemSpacing.x;
					width += 60.0f;
					width += no_button().get_size().x;
					utils::ui::imgui::AlignForWidth(width);
					return true;
				});
				yes_button().add_on_update([this](float dt) {
					ImGui::SameLine();
					ImGui::Dummy({ 60, 0 });
					ImGui::SameLine();
					return true;
				});
				return 0;
			}

			void imgui::dialog_yes_no::construct(
				const std::string& msg
				, const on_answer_t& on_answer
				, const char* yes_text
				, const char* no_text
				, const std::optional<std::string>& title
			)
			{
				base::construct(msg, on_answer, yes_text, no_text, title);
			}

			void imgui::dialog_yes_no::button_yes_show()
			{
				base::button_yes_show();
				ImGui::SameLine();
			}
		}
	}
}