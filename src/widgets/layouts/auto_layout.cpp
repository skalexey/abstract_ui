#include <abstract_ui/widgets/layouts/auto_layout.h>
#include <utils/log.h>
LOG_TITLE("utils::ui::auto_layout");

namespace utils
{
	namespace ui
	{
		auto_layout::auto_layout()
		{
			add_on_update([self = this](float dt) {
				auto& children = self->get_children();
				int children_count = children.size();
				for (int i = 0; i < children.size(); i++)
				{
					auto&& child_base = children[i];
					if (!child_base->is<ui::widget>())
					{
						LOG_ERROR("Only widgets can be added into a layout");
						assert(child_base->is<ui::widget>());
					}

					auto&& child = child_base->as<ui::widget>();
					auto&& screen_size = self->get_screen_size();
					auto&& size = child.get_size();
					if (screen_size.x > screen_size.y)
						child.set_position({(screen_size.x * i + size.x / 2) / children_count, (screen_size.y - size.y) / 2});
					else
						child.set_position({(screen_size.x - size.x) / 2, (screen_size.y * i + size.y / 2) / children_count});
				}
				return true;
			});
		}
	}
}