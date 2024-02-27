#include <QUrl>
#include <QObject>
#include <QVariantMap>
#include <abstract_ui/qt/widgets/button.h>
#include <abstract_ui/qt/widget_factory.h>
#include <utils/Log.h>

LOG_TITLE("qt::button");

namespace
{
	REGISTER_WIDGET_DECLARATIVELY(utils::ui::qt::widget_factory, utils::ui::qt::button);
}

namespace utils
{
	namespace ui
	{
		namespace qt
        {
			int qt::button::init()
			{
				const QUrl url(u"qrc:abstract_ui_qml/Button.qml"_qs);
				return qt::clickable_item::init(url);
			}
		}
	}
}
