#include <abstract_ui/widget_factory.h>
#include <abstract_ui/app.h>

namespace
{
	void set_factory(utils::ui::node* node, const utils::ui::app& app) {
		node->set_factory(app.get_factory());
	}
}
