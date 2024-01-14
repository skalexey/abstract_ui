// This class just adds the ability to work with nodes
// as a imgui::node* instead of a ui::node*

#pragma once

#include <abstract_ui/imgui/fwd.h>
#include <abstract_ui/node.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			// Hide ui::node* interface using protected inheritance
			class node : public virtual ui::node
			{
				friend class ui::widget_factory;
				using base = ui::node;

			public:
				// TODO: support constructor with imgui::node* 
				node();

				const imgui::node* get_parent() const {
					return get_typed_parent<imgui::node>();
				}
				
				imgui::node* parent() {
					return const_cast<imgui::node*>(get_parent());
				}

				imgui::node* root() {
					return dynamic_cast<imgui::node*>(base::root());
				}
				
				// All the dynamic casts require a fully defined type.
				// So we define them in the cpp file.
				imgui::app& app();
				const imgui::app& get_app() const;
			};
			using node_ptr = std::shared_ptr<imgui::node>;
		}
	}
}