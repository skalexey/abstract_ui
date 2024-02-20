#pragma once

#include <memory>
#include <abstract_ui/fwd.h>
#include <abstract_ui/node.h>
#include <abstract_ui/widget_factory.h>

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
				
				node() = default;
				
				node(const ui::node_ptr& impl) {
					set_impl(impl);
				}

				void set_impl(const ui::node_ptr& impl) {
					m_impl = impl;
					auto impl_actual_parent = impl->parent();
					if (impl_actual_parent == nullptr)
						add_node(m_impl);
					else
						assert(impl_actual_parent == static_cast<ui::node*>(this) && "Trying to call set_impl(impl) with already used impl in another node");
					on_set_impl();
				}

				// Create a widget of type T and add it as a child to this node.
				template <typename T>
				std::shared_ptr<T> create(ui::node* parent = nullptr) {
					return impl()->get_factory().template create<T>(parent ? parent : impl().get());
				}

				template <typename T>
				std::shared_ptr<T> create_abstract(ui::node* parent = nullptr, const vl::Object& options = nullptr) {
					return impl()->get_factory().template create_abstract<T>(parent ? parent : impl().get(), options);
				}

				template <typename T>
				std::shared_ptr<T> create_final() {
					return impl()->get_factory().template create_final<T>(*impl());
				}

				ui::node_ptr impl() const {
					return m_impl;
				}

				const widget_factory& get_factory() const override {
					return impl()->get_factory();
				}

			protected:
				virtual void on_set_impl() {}
				
			private:
				ui::node_ptr m_impl;
			};
		}
	}
}
