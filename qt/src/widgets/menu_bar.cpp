#include <QUrl>
#include <QWidget>
#include <QQuickItem>
#include <utils/string_utils.h>
#include <abstract_ui/widget_factory.h>
#include <abstract_ui/qt/widgets/menu_bar.h>
#include <abstract_ui/qt/widgets/menu_list.h>
#include <abstract_ui/qt/widgets/tool_button.h>
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

			void qt::menu_bar::add_node(const ui::node_ptr& node)
			{
				if (node->is<menu_list>())
				{
					auto& list = node->as<menu_list>();
					auto tool_button = ui::widget_factory::create_node<qt::tool_button>();
					tool_button->set_text(list.get_title());
					list.set_on_set_title(this, [tool_button_ptr = tool_button.get(), list_ptr = &list]() {
						tool_button_ptr->set_text(list_ptr->get_title());
					});
					auto menu_qobject = list.qobject();
					auto job = [tool_button, &list]() {
						auto menu_qobject = list.qobject();
						assert(menu_qobject && "Menu has not been initialized yet?");
						tool_button->qobject()->setProperty("menu", QVariant::fromValue(menu_qobject));
						return menu_qobject ? 0 : -1;
					};
					if (!menu_qobject)
						list.do_on_post_construct(job);
					else
						job();
					tool_button->add_node(node);
					base::add_node(tool_button);
				}
				else
				{
					base::add_node(node);
				}
			}

			void qt::menu_bar::add_item(const std::string& title, const ui::menu_list_base::action_t& action)
			{
				auto widget = ui::widget_factory::create_node<qt::tool_button>();
				widget->set_text(title);
				widget->set_on_click([action](bool) {
					action();
				});
				add_widget(widget);
			}
		}
	}
}