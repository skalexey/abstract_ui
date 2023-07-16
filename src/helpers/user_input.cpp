#include <abstract_ui/app.h>
#include <abstract_ui/widgets/dialogs/dialog_yes_no.h>
#include <abstract_ui/widgets/dialogs/dialog_message.h>
#include <abstract_ui/widgets/dialogs/dialog_input_text.h>
#include <abstract_ui/helpers/user_input.h>
#include <utils/Log.h>
#include <utils/common.h>
#include <utils/string_utils.h>

LOG_PREFIX("[user_input]: ");
LOG_POSTFIX("\n");

namespace utils
{
	namespace ui
	{
			void user_input::ask_user(
				const std::string& question
				, const utils::void_bool_cb& on_answer
				, const char* yes_btn_text
				, const char* no_btn_text
			)
			{
				LOG("ask_user: '" << question << "' ['" << utils::to_str(yes_btn_text) << "'; '" << utils::to_str(no_btn_text) << "']\n");
				auto d = m_app->get_factory().create<dialog_yes_no>(m_app);
				d->construct(
					question
					, on_answer
					, yes_btn_text ? yes_btn_text : "Yes"
					, no_btn_text ? no_btn_text : "No"
					, "Question"
				);
				d->show();
			}

			void user_input::show_message(
				const std::string& message
				, const utils::void_cb& on_close
				, const char* ok_btn_text
			)
			{
				LOG("show_message: '" << message << "' ['" << utils::to_str(ok_btn_text) << "']\n");
				auto d = m_app->get_factory().create<dialog_message>(m_app);
				d->construct(
					message
					, [=](bool) {
						if (on_close)
							on_close();
					}
					, ok_btn_text
				);
				d->show();
			}

			void user_input::ask_line(
				const std::string& msg
				, const utils::void_string_bool_cb& on_answer
				, const std::string& default_value
				, const char* ok_btn_text
				, const char* cancel_btn_text
			)
			{
				LOG("ask_line: '" << msg << "' ['" << utils::to_str(ok_btn_text) << "'; '" << utils::to_str(cancel_btn_text) << "']\n");
				auto d = m_app->get_factory().create<dialog_input_text>(m_app);
				// TODO: move this all into an Init() method.
				d->construct(
					msg
					, [=](const std::string& path, bool cancelled) {
						on_answer(path, cancelled);
					}
					, default_value
					, "Input value"
					, ok_btn_text ? ok_btn_text : "Submit"
					, cancel_btn_text
				);
				d->show();
			}

	}
}
