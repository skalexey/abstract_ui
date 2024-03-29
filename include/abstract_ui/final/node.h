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
				using base = impl_t;

				node() = default;
				
				node(const ui::node_ptr& impl) {
					set_impl(impl);
				}

				void set_impl(const ui::node_ptr& impl) {
					m_impl = impl;
					auto impl_actual_parent = impl->parent();
					if (impl_actual_parent == nullptr)
						ui::node::add_node_base(m_impl); // Avoid calling on_set_parent
					else
						assert(impl_actual_parent == static_cast<ui::node*>(this) && "Trying to call set_impl(impl) with already used impl in another node");
					on_set_impl();
				}

				void add_node(const ui::node_ptr& node) override {
					impl()->add_node(node);
				}
				
				ui::node_ptr impl() const {
					return m_impl;
				}

				const widget_factory& get_factory() const override {
					return impl()->get_factory();
				}

			protected:
				virtual void on_set_impl() {}
				void on_set_parent(ui::node* parent) override {
					base::on_set_parent(parent);
					impl()->on_set_parent_impl(parent); // TODO: think more about this
				}

				// Create a widget of type T and add it as a child to the impl node.
				template <typename T>
				std::shared_ptr<T> create(ui::node* parent = nullptr, const vl::Object& options = nullptr, ui::app* app = nullptr, bool deferred = false) {
					return impl()->get_factory().template create<T>(parent ? parent : impl().get(), options, app, deferred);
				}

				template <typename T>
				std::shared_ptr<T> create_abstract(ui::node* parent = nullptr, const vl::Object& options = nullptr) {
					return widget_factory::create_abstract<T>(parent ? parent : impl().get(), options);
				}

				template <typename T>
				std::shared_ptr<T> create_final() {
					return impl()->get_factory().template create_final<T>(*impl());
				}

			private:
				ui::node_ptr m_impl;
			};
		}
	}
}
