#include <abstract_ui/app.h>
#include <abstract_ui/window.h>
#include <abstract_ui/window_controller.h>

namespace utils
{
	namespace ui
	{
		bool window_controller::show() {
			assert(m_view);
			return m_view->show();
		}

		const widget_factory& window_controller::get_factory() const {
			return m_app.get_factory();
		}
	}
}