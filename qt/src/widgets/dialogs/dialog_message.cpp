#include <abstract_ui/qt/widget_factory.h>
#include <abstract_ui/qt/widgets/dialogs/dialog_message.h>
#include <abstract_ui/qt/common.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			REGISTER_WIDGET_DECLARATIVELY(qt::dialog_message);

			void qt::dialog_message::button_ok_show()
			{
				base::button_ok_show();
			}
		}
	}
}