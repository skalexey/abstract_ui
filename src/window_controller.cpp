#include <abstract_ui/app.h>
#include <abstract_ui/window.h>
#include <abstract_ui/window_controller.h>

namespace utils
{
	namespace ui
	{
		window_controller::window_controller()
		{
			do_on_post_construct([self = this]() {
				auto& app = self->app();
				auto ptr = self->shared_from_this();
				app.add_node(ptr);
				return 0;
			});
		}
	}
}