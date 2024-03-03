#pragma once

#include <string>
#include <abstract_ui/imgui/fwd.h>
#include <abstract_ui/widgets/text_input.h>
#include <abstract_ui/imgui/widget.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class text_input : public imgui::widget, public ui::text_input
			{
			public:
				using base = ui::text_input;
				text_input() {
					m_edit_value.reserve(256);
					m_input_label = "##" + std::to_string(next_id());
				}

				const std::string& get_value() const override;
				void set_value(const std::string& value) override;

			protected:
				bool update_input(float dt) override;
				bool update_text(float dt) override;
				void on_set_label() override;

			private:
				std::string m_input_label;
				std::string m_edit_value;
			};
		}
	}
}