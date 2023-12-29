#include <cmath>
#include <imgui.h>
#include <imgui_internal.h>
#include <abstract_ui/imgui/widgets/button.h>
#include <abstract_ui/imgui/widget_factory.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			REGISTER_WIDGET_DECLARATIVELY(imgui::widget_factory, imgui::button);

			bool button::on_update(float dt)
			{
				if (!imgui_button_update(dt))
					return false;
				if (!ui::button::on_update(dt))
					return false;
				if (!imgui::widget::on_update(dt))
					return false;
				return true;	
			}

			bool button::imgui_button_update(float dt)
			{
				#ifdef ANDROID
					auto ts = text_size();
					vec2i new_size = {std::max(100, ts.x + 20), 100};
					set_size(new_size);
					const ImVec2 sz = {float(new_size.x), float(new_size.y)};
				#else
					const ImVec2 sz = {0, 0};
				#endif
				if (!m_enabled)
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				if (ImGui::Button(get_text().c_str(), sz))
					if (m_enabled)
						on_click(true);
				auto result = on_imgui_button_update(dt);
				if (!m_enabled)
					ImGui::PopItemFlag();
				return result;
			}

			const vec2i& button::text_size()
			{
				auto s = ImGui::CalcTextSize(get_text().c_str());
				return m_calculated_size = { (int)s.x, (int)s.y };
			}
		}
	}
}