#pragma once

#include <memory>
#include <string>
#include <abstract_ui/imgui/fwd.h>
#include <abstract_ui/widgets/text.h>
#include <abstract_ui/imgui/widget.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class text : public imgui::widget, public ui::text
			{
			public:
				using base = ui::text;

				void set_text(const std::string& text) {
					m_text = text;
				}

				const std::string& get_text() const {
					return m_text;
				}

			protected:
				bool imgui_text_update(float dt);
				
				virtual bool on_imgui_text_update(float dt) {
					return true;
				}
				
			private:
				bool on_update(float dt) override final {
					if (!imgui_text_update(dt))
						return false;
					return imgui::widget::on_update(dt);
				}

			private:
				std::string m_text;
			};
			using text_ptr = std::shared_ptr<text>;
		}
	}
}