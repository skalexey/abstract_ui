#include <abstract_ui/qt/widget_factory.h>
#include <abstract_ui/qt/widgets/dialogs/dialog_message.h>
#include <abstract_ui/qt/common.h>

namespace
{
	REGISTER_WIDGET_DECLARATIVELY(utils::ui::qt::widget_factory, utils::ui::qt::dialog_message);
}

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			void qt::dialog_message::button_ok_show()
			{
				base::button_ok_show();
			}
		}
	}
}