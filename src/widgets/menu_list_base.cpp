#include <abstract_ui/widget_factory.h>
#include <abstract_ui/widgets/menu_item.h>
#include <abstract_ui/widgets/menu_list_base.h>
#include <utils/Log.h>
LOG_TITLE("menu_list_base");

namespace utils
{
	namespace ui
	{
		void menu_list_base::add_item(const std::string& title, const action_t& action)
		{
			auto widget = get_factory().create<ui::menu_item>();
			widget->set_text(title);
			widget->set_on_click([action](bool) {
				action();
			});
			add_widget(widget);
		}
	}
}