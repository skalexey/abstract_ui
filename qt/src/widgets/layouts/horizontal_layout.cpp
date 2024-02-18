#include <QUrl>
#include <abstract_ui/qt/widgets/layouts/horizontal_layout.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			int qt::horizontal_layout::init()
			{
				const QUrl url(u"qrc:abstract_ui_qml/HorizontalLayout.qml"_qs);
				return qt::widget::init(url);
			}
		}
	}
}