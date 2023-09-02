#include <abstract_ui/qt/widget_factory.h>
#include <abstract_ui/qt/widgets/dialogs/dialog_yes_no.h>
#include <abstract_ui/qt/common.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			REGISTER_WIDGET_DECLARATIVELY(qt::dialog_yes_no);

			void qt::dialog_yes_no::construct(
				const std::string& msg
				, const on_answer_t& on_answer
				, const char* yes_text
				, const char* no_text
				, const std::optional<std::string>& title
			)
			{
				base::construct(msg, on_answer, yes_text, no_text, title);
			}

			void qt::dialog_yes_no::button_yes_show()
			{
				base::button_yes_show();
				// qt::SameLine();
			}
		}
	}
}