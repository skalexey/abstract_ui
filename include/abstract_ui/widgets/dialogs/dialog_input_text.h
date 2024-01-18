#pragma once

#include <vector>
#include <string>
#include <abstract_ui/widgets/dialogs/modal_dialog.h>
#include <abstract_ui/widgets/button.h>
#include <abstract_ui/widgets/label.h>
#include <abstract_ui/widgets/text_input.h>
#include <abstract_ui/widget_factory.h>
#include <utils/common_macros.h>

namespace utils
{
	namespace ui
	{
		class dialog_input_text : public virtual modal_dialog
		{
			using base = dialog;

			public:
				using on_result_t = std::function<void(const std::string& result, bool cancelled)>;

				dialog_input_text()
				{
					do_on_post_construct([self = this] {
						return self->this_on_post_construct();
					});
				}

				// In place of a constructor as we only support default one
				virtual void construct(
				    const std::string& msg
					, const on_result_t& on_result = {}
					, const std::string& default_input_text = {}
					, const std::string& title = {}
					, const char* ok_btn_text = nullptr
					, const char* cancel_btn_text = nullptr
				)
				{
					set_on_result(on_result);
					m_message_label->set_text(msg);
					m_text_input->set_value(default_input_text);
					// TODO: remove this label
					m_text_input->set_label(title);
					m_ok_button->set_text(ok_btn_text ? ok_btn_text : "Ok");
					m_cancel_button->set_text(cancel_btn_text ? cancel_btn_text : "Cancel");
				}

				label& message_label() { return *m_message_label; }
				ui::text_input& text_input() { return *m_text_input; }
				button& ok_button() { return *m_ok_button; }
				button& cancel_button() { return *m_cancel_button; }

				void set_on_result(const on_result_t& on_result) {
					m_on_result = on_result;
				}
				
			private:
				int this_on_post_construct()
				{
					set_title("Provide your input");
					m_message_label = get_factory().create<ui::label>(this);
					m_text_input = get_factory().create<ui::text_input>(this);
					m_ok_button = get_factory().create<ui::button>(this);
					m_cancel_button = get_factory().create<ui::button>(this);
					auto on_submit = [self = this](const std::string& new_value) {
						if (self->m_on_result)
							self->m_on_result(new_value, false);
						self->close();
					};
					m_ok_button->set_on_click([self = this, on_submit](bool up) {
						if (!up)
                            return false;
						on_submit(self->m_text_input->get_value());
                        return true;
					});
					m_text_input->set_on_new_value([on_submit](const std::string& new_value) {
						on_submit(new_value);
					});
					m_cancel_button->set_on_click([this](bool up) {
						if (m_on_result)
							m_on_result(m_text_input->get_value(), true);
						close();
					});
					add_on_update([self = this](float dt) {
						self->set_position_relative({ 0.5f, 0.5f }, { 0.5f, 0.5f });
						return true;
					});
					return 0;
				}

			protected:
				label_ptr m_message_label;
				button_ptr m_ok_button;
				button_ptr m_cancel_button;
				text_input_ptr m_text_input;
				on_result_t m_on_result;
		};
		using dialog_input_text_ptr = std::unique_ptr<dialog_input_text>;
	}
}
