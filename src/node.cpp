#include <cassert>
#include <abstract_ui/node.h>
#include <abstract_ui/app.h>
#include <abstract_ui/final/node.h>
#include <utils/Log.h>
LOG_TITLE("utils::ui::node");

namespace utils
{
	namespace ui
	{
		const ui::app& ui::node::get_app() const
		{
			if (m_app)
				return *m_app;
			else
				return ui::app::get();
		}

		const node* ui::node::get_impl_parent() const
		{
			auto final_node = dynamic_cast<const final::node*>(m_parent);
			if (final_node)
			{
				auto impl = final_node->impl().get();
				if (this == impl)
					return final_node->get_parent();
				else
					return impl;
			}
			return m_parent;
		}

		// All the dynamic casts require a fully defined type.
		void ui::node::add_node(const node_ptr& node)
		{
			// TODO: support concurrency
			assert(node && "Adding a null node");
			assert(node.get() && "Adding a half-null node");
			assert(node->m_parent == nullptr && "Adding a node that already has a parent");
			// This code is called from parent to avoid having a potentially called method
			node->m_parent = this;
			node->m_app = dynamic_cast<ui::app*>(node->root());
			m_children.push_back(node);
			on_add_node(node.get());
			node->on_set_parent(this);
		}
	}
}
