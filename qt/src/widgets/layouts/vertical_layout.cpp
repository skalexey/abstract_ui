#include <QUrl>
#include <abstract_ui/qt/widgets/layouts/vertical_layout.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			int qt::vertical_layout::init()
			{
				const QUrl url(u"qrc:abstract_ui_qml/VerticalLayout.qml"_qs);
				return qt::widget::init(url);
			}
		}
	}
}