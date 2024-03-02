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
#include <VL.h>
#include <utils/ordered_map.h>
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
			friend class final::node;
		public:
			virtual ~node() {
				destroy();
			}

			void set_options(const vl::Object& options) {
				m_options = options;
				on_set_options();
			}

			const vl::Object& get_options() const {
				return m_options;
			}

			int post_construct() {
				RETURN_IF_NE_0(post_construct_1());
				return on_after_post_construct();
			}

			virtual void do_on_post_construct(const utils::int_cb& cb) {
				m_on_post_construct.push_back(cb);
			}

			virtual bool process_event(const std::string& name, const vl::Object& data) {
				return false;
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

			virtual void add_node(const node_ptr& node);
			virtual void add_node_base(const node_ptr& node);

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
				// A child can be removed during iteration, so we need to iterate through a copy.
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
				m_added_on_update.push_back(on_update);
			}

			void set_on_before_update(const on_update_t& on_before_update) {
				m_on_before_update = on_before_update;
			}

			void set_factory(const widget_factory& factory) {
				m_factory = const_cast<widget_factory*>(&factory);
			}
			
			virtual const widget_factory& get_factory() const {
				assert(m_factory && "Forgot to call set_factory()?");
				return *m_factory;
			}

			const ui::app& get_app() const;

			virtual int init() {
				return 0;
			}

			virtual bool is_initialized() const {
				return true;
			}

			bool_cb& add_on_set_parent(const void* subscriber, const bool_cb& cb) {
				return m_on_set_parent.add(subscriber, cb);
			}

			bool clear_on_set_parent(const void* subscriber) {
				return m_on_set_parent.erase(subscriber);
			}

			const std::vector<node_ptr>& get_children() const {
				return m_children;
			}

			std::vector<node_ptr>& children() {
				return m_children;
			}

		protected:
			virtual void on_set_options() {}
			virtual int post_construct_1() {
				// Other implementations may want to call it from another thread (e.g. Qt)
				RETURN_IF_NE_0(init());
				RETURN_IF_NE_0(on_post_construct());
				for (auto&& cb : m_on_post_construct)
					RETURN_IF_NE_0(cb());
				return 0;
			}
			// !!!Attention!!!
			// Call this method in the destructor of every class that overrides on_destroy()
			// to avoid calling the virtual method on a destroyed object.
			void destroy() {
				foreach_child<node>([](node* child) {
					// TODO: support concurrency
					child->destroy();
					return true;
				});
				on_destroy();
				remove_from_parent();
			}
			virtual void on_destroy() {};
			template <typename T>
			const T* get_typed_parent() const {
				auto base_parent = get_impl_parent();
				auto casted_parent = dynamic_cast<const T*>(base_parent);
				assert(base_parent == casted_parent && "The parent is not a qt::node*");
				return casted_parent;
			}
			
			virtual int on_post_construct() {
				return 0;
			}

			virtual int on_after_post_construct() {
				return 0;
			}
			
			virtual bool on_before_update(float dt) {
				if (m_on_before_update)
					return m_on_before_update(dt);
				return true;
			}
			
			virtual bool update_child(ui::node* const child, float dt) {
				assert(child);
				if (!child->update(dt))
					return false;
				return true;
			}

			virtual bool update_children(float dt) {
				return foreach_child<ui::node>([self = this, dt](ui::node* child) {
					return self->update_child(child, dt);
				});
			}

			bool user_update(float dt) {
				if (!m_added_on_update.empty())
				{
					m_on_update.insert(m_on_update.end(), m_added_on_update.begin(), m_added_on_update.end());
					m_added_on_update.clear();
				}
				for (auto&& m_on_update : m_on_update)
					if (!m_on_update(dt))
						return false;
				return true;
			}
			
			virtual void on_add_node(node* node) {}
			virtual void on_set_parent(ui::node* parent) {}
			void on_set_parent_impl(ui::node* parent) {
				on_set_parent(parent);
				for(auto it = m_on_set_parent.begin(); it != m_on_set_parent.end();)
					if (!(*it).second())
						it = m_on_set_parent.erase(it);
					else
						++it;
			}

		private:
			node* m_parent = nullptr;
			std::vector<node_ptr> m_children;
			ui::app* m_app = nullptr;
			widget_factory* m_factory = nullptr;
			on_update_t m_on_before_update;
			std::vector<on_update_t> m_on_update;
			std::vector<on_update_t> m_added_on_update;
			std::vector<utils::int_cb> m_on_post_construct;
			utils::ordered_map<const void*, bool_cb> m_on_set_parent;
			vl::Object m_options = nullptr;
		};
		using node_ptr = std::shared_ptr<node>;
	}
}
