#include <QUrl>
#include <utils/log.h>
#include <utils/string_utils.h>
#include <abstract_ui/qt/widgets/menu_item.h>
LOG_TITLE("qt::menu_item");

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			int qt::menu_item::init()
			{
				QUrl url(u"qrc:abstract_ui_qml/MenuItem.qml"_qs);
				return qt::clickable_item::init(url);
			}
		}
	}
}