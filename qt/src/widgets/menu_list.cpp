#include <QUrl>
#include <QMenu>
#include <QQuickItem>
#include <utils/string_utils.h>
#include <abstract_ui/widget_factory.h>
#include <abstract_ui/widgets/menu_item.h>
#include <abstract_ui/qt/widgets/menu_list_model.h>
#include <abstract_ui/qt/widgets/menu_list.h>
#include <utils/log.h>
LOG_TITLE("qt::menu_list");

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			int qt::menu_list::init()
			{
				const QUrl url(u"qrc:abstract_ui_qml/MenuList.qml"_qs);
				QVariantMap initial_properties;
				initial_properties["title"] = QString(get_title().c_str());
				return qt::widget::init(url, initial_properties);
			}

			void qt::menu_list::on_set_title()
			{
				qobject()->setProperty("title", QString(get_title().c_str()));
			}

			widget_model* qt::menu_list::create_model() const
			{
				return new menu_list_model();
			}

			void qt::menu_list::add_widget(const ui::widget_ptr& widget)
			{
				add_node(widget);
				QQuickItem* item = qobject_cast<QQuickItem*>(widget->as<qt::node>().qobject());
				assert(item && "widget is not a QML type");
				QMetaObject::invokeMethod(qobject(), "addItem", Q_ARG(QQuickItem*, item));
			}
		}
	}
}
