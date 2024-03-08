#include <abstract_ui/menu_controller.h>
#include <abstract_ui/widgets/menu.h>
#include <utils/log.h>
LOG_TITLE("menu_controller");
namespace utils
{
	namespace ui
	{
		void menu_controller::close()
		{
			view().close();
			remove_from_parent();
		}
	}
}