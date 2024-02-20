#include <abstract_ui/menu_controller.h>
#include <abstract_ui/widgets/menu.h>
#include <utils/Log.h>
LOG_TITLE("menu_controller");
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