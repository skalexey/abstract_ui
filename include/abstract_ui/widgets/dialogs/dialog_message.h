#pragma once

#include <string>
#include <abstract_ui/widgets/dialogs/dialog_with_buttons.h>
#include <utils/common.h>

namespace utils
{
	namespace ui
	{
		class dialog_message : public dialog_with_buttons
		{
			const std::string ok_text_default = "Ok";

			using base = dialog_with_buttons;

			public:
				using on_answer_t = utils::void_bool_cb;

				int on_post_construct() override {
					set_title("Message");
					add_button();
					ok_button().set_on_click([this](bool up) {
						this->on_answer(true);
					});
					return 0;
				}
				
				// In place of a constructor as we only support default one
				virtual void construct(
				    const std::string& msg
					, const on_answer_t& on_answer
					, const char* ok_text
					, const std::string& title = {}
				)
				{
					set_title(title);
					set_message(msg);
					set_on_answer(on_answer);
					ok_button().set_text(ok_text ? ok_text : ok_text_default.c_str());
				}

				void set_on_answer(const on_answer_t& on_answer) {
					m_on_answer = on_answer;
				}
				void set_ok_text(const std::string& text) {
					ok_button().set_text(text);
				}

				button& ok_button() { return *m_buttons[0]; }

			protected:
				virtual void button_ok_show() {
					ok_button().show();
				}

				virtual void message_label_show() {
					message_text().show();
				}

				void on_show() override {
					message_label_show();
					button_ok_show();
				}

				void on_answer(bool answer) {
					m_on_answer(answer);
					close();
				}

			private:
				on_answer_t m_on_answer;
		};
	}
}