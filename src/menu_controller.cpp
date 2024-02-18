#include <abstract_ui/menu_controller.h>
#include <abstract_ui/widgets/menu.h>

namespace utils
{
	namespace ui
	{
		void menu_controller::close()
		{
			get_view()->close();
			remove_from_parent();
		}
	}
}