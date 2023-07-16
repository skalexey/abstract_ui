#include <cassert>
#include <abstract_ui/node.h>
#include <abstract_ui/app.h>

namespace utils
{
	namespace ui
	{
		// All the dynamic casts require a fully defined type.
		void ui::node::add_node(const node_ptr& node)
		{
			assert(node && "Adding a null node");
			assert(node.get() && "Adding a half-null node");
			// This code is called from parent to avoid having a potentially called method
			node->m_parent = this;
			node->m_app = dynamic_cast<ui::app*>(node->root());
			m_children.push_back(node);
		}
	}
}
