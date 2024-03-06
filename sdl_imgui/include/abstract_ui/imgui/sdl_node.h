// This class just adds the ability to work with sdl_nodes
// as a imgui::sdl_node* instead of a ui::sdl_node*

#pragma once

#include <abstract_ui/imgui/fwd.h>
#include <abstract_ui/imgui/node.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class sdl_node : public virtual imgui::node
			{
				using base = imgui::node;

			public:
				const imgui::sdl_node* get_parent() const {
					return get_typed_parent<imgui::sdl_node>();
				}
				
				imgui::sdl_node* parent() {
					return const_cast<imgui::sdl_node*>(get_parent());
				}

				imgui::sdl_node* root() {
					return dynamic_cast<imgui::sdl_node*>(base::root());
				}
				
				// All the dynamic casts require a fully defined type.
				// So we define them in the cpp file.
				imgui::sdl_app& app();
				const imgui::sdl_app& get_app() const;
			};
			using sdl_node_ptr = std::shared_ptr<imgui::sdl_node>;
		}
	}
}