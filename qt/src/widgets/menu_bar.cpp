#include <QUrl>
#include <QQuickItem>
#include <utils/string_utils.h>
#include <abstract_ui/qt/widgets/menu_bar.h>
#include <abstract_ui/widget_factory.h>
#include <abstract_ui/widgets/menu_item.h>
#include <utils/Log.h>
LOG_TITLE("qt::menu_bar");

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			int qt::menu_bar::init()
			{
				const QUrl url(u"qrc:abstract_ui_qml/MenuBar.qml"_qs);
				return qt::widget::init(url);
			}

			// void qt::menu_bar::add_item(const std::string& title, const action_t& action)
			// {
			// 	auto widget = get_factory().create<ui::menu_item>();
			// 	add_widget(widget);
			// 	widget->set_text(title);
			// 	widget->set_on_click([action](bool) {
			// 		action();
			// 	});
			// }

			// void qt::menu_bar::add_widget(const ui::widget_ptr& widget)
			// {
			// 	QQuickItem *item = qobject_cast<QQuickItem*>(widget->as<qt::node>().qobject());
			// 	assert(item && "widget is not a QML type");
			// 	QMetaObject::invokeMethod(qobject(), "addItem", Q_ARG(QQuickItem*, item));
			// 	base::add_widget(widget);
			// }
		}
	}
}