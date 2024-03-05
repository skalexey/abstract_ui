#include <SDL3/SDL.h>
#include <abstract_ui/widgets/text_input.h>
#include <utils/io_utils.h>
#include <abstract_ui/imgui/widgets/text_input.h>
#include <abstract_ui/imgui/sdl_app.h>
#include <abstract_ui/imgui/widget_factory.h>

namespace
{
	REGISTER_WIDGET_DECLARATIVELY(utils::ui::imgui::widget_factory, utils::ui::imgui::text_input);
}

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
            bool imgui::text_input::update_input(float dt)
			{
				// TODO: do smth with this resize:
				m_edit_value.resize(strlen(m_edit_value.c_str()));
				ImGui::InputText(m_input_label.c_str(), m_edit_value.data(), m_edit_value.capacity()); // Return value is not used
				// TODO: move it to imgui::widget class:
				// Store size and position of ImGui widget for further getting it with get_size() and get_position().
				auto r_min = ImGui::GetItemRectMin();
				auto r_max = ImGui::GetItemRectMax();
				// Save text_input::set_size and set_position for actual resizing and repositioning.
				base::set_size(vec2i(r_max.x - r_min.x, r_max.y - r_min.y));
				base::set_position(vec2i(r_min.x, r_min.y));
				auto& io = ImGui::GetIO();
				if (io.WantTextInput)
				{
					auto& p = get_position();
					auto& s = get_size();
					// TODO: abstract
					SDL_Rect rec = { p.x, p.y, s.x, s.y };
					SDL_SetTextInputRect(&rec);
					sdl_app::request_keyboard();
				}
				return true;
			}

			bool imgui::text_input::update_text(float dt)
			{
				ImGui::Text("%s", get_label().c_str());
				ImGui::SameLine();
				return true;
			}

			void imgui::text_input::on_set_label()
			{
				m_input_label = utils::format_str("%s##Answer", get_label().c_str());
			}

			const std::string& imgui::text_input::get_value() const
			{
				return m_edit_value;
			}

			void imgui::text_input::set_value(const std::string& value)
			{
				m_edit_value = value;
			}
		}
	}
}