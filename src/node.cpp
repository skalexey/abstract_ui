#include <cassert>
#include <abstract_ui/node.h>
#include <abstract_ui/app.h>
#include <abstract_ui/final/node.h>

namespace utils
{
	namespace ui
	{
		const node* ui::node::get_parent() const {
			auto final_node = dynamic_cast<const final::node*>(m_parent);
			if (final_node)
				return final_node->impl().get();
			return m_parent;
		}

		// All the dynamic casts require a fully defined type.
		void ui::node::add_node(const node_ptr& node)
		{
			// TODO: support concurrency
			assert(node && "Adding a null node");
			assert(node.get() && "Adding a half-null node");
			// This code is called from parent to avoid having a potentially called method
			node->m_parent = this;
			node->m_app = dynamic_cast<ui::app*>(node->root());
			m_children.push_back(node);
		}
	}
}
