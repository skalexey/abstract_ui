#pragma once

#include <memory>
#include <abstract_ui/imgui/fwd.h>
#include <abstract_ui/widgets/button.h>
#include <abstract_ui/imgui/widget.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class button : public imgui::widget, public ui::button
			{
			public:
				using base = ui::button;
				
				const vec2i& text_size();

				const vec2i& get_size() {
					return m_calculated_size;
				}

				void set_enabled(bool enabled) override {
					m_enabled = enabled;
				}

			protected:
				virtual bool on_imgui_button_update(float dt) {
					return true;
				}

			private:
				bool imgui_button_update(float dt);
				bool on_update(float dt) override final;

			private:
				vec2i m_calculated_size;
				bool m_enabled = true;
			};
			using button_ptr = std::shared_ptr<button>;
		}
	}
}