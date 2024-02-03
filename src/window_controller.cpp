#include <abstract_ui/app.h>
#include <abstract_ui/window.h>
#include <abstract_ui/window_controller.h>

namespace utils
{
	namespace ui
	{
		window_controller::window_controller(utils::ui::app& app)
		{
			set_factory(app.get_factory()); // Window controllers are implementation-abstract.
			do_on_post_construct([&, self = this]() {
				auto ptr = self->shared_from_this();
				app.add_node(ptr);
				return 0;
			});
		}
	}
}