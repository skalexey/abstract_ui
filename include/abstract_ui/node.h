// ui/node.h
// 
// Description:
// Base class for all UI elements and the application.
// 
// Setting parent:
// We do not pass parent to the constructor, but use deferred parent set instead for several reasons:
//	1. We want to store child nodes as shared pointers.
//	2. To avoid necessity of explicitly specifying all the base classes constructor calls and keep this process automatic
//	   as we have lots of classes in the inheritance tree and we widely use virtual inheritance.

#pragma once

#include <vector>
#include <cassert>
#include <memory>
#include <functional>
#include <abstract_ui/fwd.h>
#include <abstract_ui/entity.h>
#include <utils/common.h>
#include <utils/common_macros.h>

namespace utils
{
	namespace ui
	{
		class node : public virtual entity
		{
			friend class widget_factory;

		public:
			virtual ~node() {
				remove_from_parent();
				foreach_child<node>([](node* child) {
					// TODO: support concurrency
					child->m_parent = nullptr;
					return true;
				});
			}

			int post_construct() {
				RETURN_IF_NE_0(on_post_construct());
				return on_after_post_construct();
			}

			virtual void do_on_post_construct(const utils::int_cb& cb) {
				m_on_post_construct.push_back(cb);
			}

			bool remove_node(node* node) {
				// TODO: support concurrency
				for (auto it = m_children.begin(); it != m_children.end(); ++it)
					if (it->get() == node) {
						node->m_parent = nullptr;
						m_children.erase(it);
						return true;
					}
				return false;
			}

			void add_node(const node_ptr& node);

			virtual void on_before_remove_from_parent() {}

			bool remove_from_parent() {
				on_before_remove_from_parent();
				if (m_parent)
					m_parent->remove_node(this);
				else
					return false;
				return true;
			}
			
			template <typename T>
			using each_child_pred_t = std::function<bool(T*)>;
			template <typename T>
			bool foreach_child(const each_child_pred_t<T>& cb) {
				// A child can be removed during iteration, so we need iterate through a copy.
				auto children = m_children;
				for (auto child : children)
				{
					assert(child);
					if (!cb(dynamic_cast<T*>(child.get())))
						return false;
				}
				return true;
			}

			const node* get_parent() const {
				return m_parent;
			}

			const node* get_impl_parent() const;

			node* parent() {
				return const_cast<node*>(get_parent());
			}

			node* impl_parent() {
				return const_cast<node*>(get_impl_parent());
			}

			node* root() {
				return m_parent ? m_parent->root() : this;
			}

            ui::app& app() {
				return const_cast<ui::app&>(const_cast<const node*>(this)->get_app());
			}

			virtual bool update(float dt) {
				if (!on_before_update(dt))
					return false;
				if (!update_children(dt))
					return false;
				if (!on_update(dt))
					return false;
				if (!user_update(dt))
					return false;
				return true;
			}

			virtual bool on_update(float dt) {
				return true;
			}

			using on_update_t = std::function<bool(float)>;

			void add_on_update(const on_update_t& on_update) {
				m_on_update.push_back(on_update);
			}

			void set_on_before_update(const on_update_t& on_before_update) {
				m_on_before_update = on_before_update;
			}

			void set_factory(widget_factory& factory) {
				m_factory = &factory;
			}
			
			const widget_factory& get_factory() const {
				assert(m_factory && "Forgot to call set_factory()?");
				return *m_factory;
			}

			const ui::app& get_app() const;

			virtual int init() {
				return 0;
			}

		protected:
			template <typename T>
			const T* get_typed_parent() const {
				auto base_parent = get_impl_parent();
				auto casted_parent = dynamic_cast<const T*>(base_parent);
				assert(base_parent == casted_parent && "The parent is not a qt::node*");
				return casted_parent;
			}
			
			virtual int on_post_construct() {
				RETURN_IF_NE_0(init());
				// We need to keep it here to be able to overload and call it from another thread (ex. for Qt)
				for (auto&& cb : m_on_post_construct)
					RETURN_IF_NE_0(cb());
				return 0;
			}

			virtual int on_after_post_construct() {
				auto parent_ptr = parent();
				if (parent_ptr)
					parent_ptr->on_add_node(this);
				on_set_parent(parent_ptr);
				return 0;
			}
			
			virtual bool on_before_update(float dt) {
				if (m_on_before_update)
					return m_on_before_update(dt);
				return true;
			}
			
			virtual bool update_children(float dt) {
				return foreach_child<ui::node>([dt](ui::node* child) {
					assert(child);
					child->update(dt);
					return true;
				});
			}

			bool user_update(float dt) {
				for (auto&& m_on_update : m_on_update)
					if (!m_on_update(dt))
						return false;
				return true;
			}

			virtual void on_add_node(node* node) {}
			virtual void on_set_parent(const ui::node* parent) {}

		private:
			node* m_parent = nullptr;
			std::vector<node_ptr> m_children;
			ui::app* m_app = nullptr;
			widget_factory* m_factory = nullptr;
			on_update_t m_on_before_update;
			std::vector<on_update_t> m_on_update;
			std::vector<utils::int_cb> m_on_post_construct;
		};
		using node_ptr = std::shared_ptr<node>;
	}
}
