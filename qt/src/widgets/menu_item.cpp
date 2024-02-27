#include <QUrl>
#include <utils/string_utils.h>
#include <abstract_ui/qt/widgets/menu_item.h>
#include <abstract_ui/qt/widgets/menu_bar.h>
#include <utils/Log.h>
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
				if (auto parent = get_parent())
					if (parent->is<qt::menu_bar>())
						url = QUrl(u"qrc:abstract_ui_qml/ToolButton.qml"_qs);
				return qt::widget::init(url);
			}
		}
	}
}