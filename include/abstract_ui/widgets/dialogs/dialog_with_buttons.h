#pragma once

#include <vector>
#include <string>
#include <optional>
#include <abstract_ui/widgets/dialogs/modal_dialog.h>
#include <abstract_ui/widgets/button.h>
#include <abstract_ui/widgets/text.h>
#include <abstract_ui/widget_factory.h>

namespace utils
{
	namespace ui
	{
		class dialog_with_buttons : public virtual modal_dialog
		{
		public:
			using actions_t = std::vector<std::pair<std::string, button::on_click_t>>;

			// In place of a constructor as we only support default one
			virtual void construct(
			    const std::string& msg
				, const actions_t& actions
				, const std::optional<std::string>& title = {}
			)
			{
				if (title.has_value())
					set_title(title.value());
				set_message(msg);
				setup_buttons(actions);
			}

			void setup_buttons(const actions_t& actions) {
				for (auto& action : actions)
					add_button(action.first, action.second);
			}

			text& message_text() { return *m_text_message; }

			void add_button(const std::string& text, const button::on_click_t& on_click = nullptr) {
				auto btn = get_factory().create<ui::button>(this);
				btn->set_text(text);
				btn->set_on_click(on_click);
				add_button(btn);
			}
			void set_message(const std::string& message)
			{
				if (!m_text_message)
					m_text_message = get_factory().create<ui::text>(this);
				m_text_message->set_text(message);
			}

		protected:
			int init() override
			{
				set_auto_resize(true);
				set_title("Dialog With Buttons");
				// Factory here is supposed to be already set in the implementation class
				set_message_text(get_factory().create<ui::text>(this));
				return 0;
			}
			
			void add_button(const ui::button_ptr& btn) {
				m_buttons.push_back(btn);
			}
			void set_message_text(const text_ptr& lbl) {
				m_text_message = lbl;
			}

		protected:
			std::vector<button_ptr> m_buttons;

		private:
			text_ptr m_text_message;
		};
		using dialog_with_buttons_ptr = std::unique_ptr<dialog_with_buttons>;
	}
}