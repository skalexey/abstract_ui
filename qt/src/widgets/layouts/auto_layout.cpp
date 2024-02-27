#include <QUrl>
#include <abstract_ui/qt/widgets/layouts/auto_layout.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			int qt::auto_layout::init()
			{
				const QUrl url(u"qrc:abstract_ui_qml/AutoLayout.qml"_qs);
				return qt::widget::init(url);
			}
		}
	}
}