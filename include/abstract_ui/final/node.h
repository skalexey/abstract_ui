#pragma once

#include <abstract_ui/fwd.h>
#include <abstract_ui/node.h>

namespace utils
{
	namespace ui
	{
		namespace final
		{
			class node : public virtual ui::node
			{
			public:
				using impl_t = ui::node;

				node(const ui::node_ptr& impl) {
					m_impl = impl;
					add_node(m_impl);
				}

				// Create a widget of type T and add it as a child to this node.
				template <typename T>
				std::shared_ptr<T> create() {
					return impl()->get_factory().create<T>(impl().get());
				}

				ui::node_ptr impl() {
					return m_impl;
				}

				const ui::node_ptr& get_impl() const {
					return m_impl;
				}

			private:
				ui::node_ptr m_impl;
			};
		}
	}
}
