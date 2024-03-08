#include <QUrl>
#include <utils/log.h>
#include <utils/string_utils.h>
#include <abstract_ui/qt/widgets/tool_button.h>
LOG_TITLE("qt::tool_button");

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			int qt::tool_button::init()
			{
				QUrl url(u"qrc:abstract_ui_qml/ToolButton.qml"_qs);
				return qt::clickable_item::init(url);
			}
		}
	}
}