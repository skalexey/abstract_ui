#include <abstract_ui/qt/widget_factory.h>
#include <abstract_ui/qt/widgets/dialogs/dialog_yes_no.h>
#include <abstract_ui/qt/common.h>

namespace
{
	REGISTER_WIDGET_DECLARATIVELY(utils::ui::qt::widget_factory, utils::ui::qt::dialog_yes_no);
}

namespace utils
{
	namespace ui
	{
		namespace qt
		{
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