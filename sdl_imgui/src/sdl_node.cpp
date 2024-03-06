#include <abstract_ui/imgui/sdl_app.h>
#include <abstract_ui/imgui/widget_factory.h>
#include <abstract_ui/imgui/sdl_node.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			// All the dynamic casts require a fully defined type.
			imgui::sdl_app& imgui::sdl_node::app()
			{
				return dynamic_cast<imgui::sdl_app&>(base::app());
			}

			const imgui::sdl_app& imgui::sdl_node::get_app() const
			{
				return dynamic_cast<const imgui::sdl_app&>(base::get_app());
			}
		}
	}
}