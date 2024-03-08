#include <QUrl>
#include <utils/string_utils.h>
#include <abstract_ui/qt/widgets/menu.h>
#include <utils/log.h>
LOG_TITLE("qt::menu");

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			int menu::init()
			{
				const QUrl url(u"qrc:abstract_ui_qml/Menu.qml"_qs);
				QVariantMap initial_properties;
				initial_properties["title"] = QString(get_title().c_str());
				return qt::widget::init(url, initial_properties);
			}

			void menu::on_set_title()
			{
				qobject()->setProperty("title", QString(get_title().c_str()));
			}
		}
	}
}