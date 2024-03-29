
#pragma once
#include <cassert>
#include <memory>
#include <VL.h>
#include <abstract_ui/widget_factory.h>
#include <abstract_ui/fwd.h>
#include <abstract_ui/node.h>

namespace utils
{
	namespace ui
	{
		class view_controller : public virtual node
		{
		public:
			using base = node;
			~view_controller() override {
				if (m_view)
					m_view->remove_from_parent();
			}
			
			template <typename T>
			std::shared_ptr<T> create_child_view_controller(const vl::Object& options = nullptr) {
				vl::Object final_options = options ? options : vl::Object();
				if (!options.Has("view_parent"))
					final_options["view_parent"] = &view().as<ui::node>();
				return widget_factory::create_abstract<T>(this, final_options);
			}

			const base& get_view() const {
				return *m_view;
			}

			base& view() {
				return *m_view;
			}

			void close() {
				if (m_view)
					m_view->remove_from_parent();
			}

			template <typename T>
			void set_view(ui::node* parent = nullptr) {
				auto final_parent = parent;
				auto&& options = get_options();
				if (parent == nullptr)
					if (options.Has("view_parent"))
						final_parent = options["view_parent"].as<vl::Pointer>().Val<ui::node>();
				m_view = std::dynamic_pointer_cast<base>(get_factory().create_final<T>(*final_parent, options));
			}

			// Create a widget of type T and add it as a child to the view.
			template <typename T>
			std::shared_ptr<T> create(ui::node* parent = nullptr, const vl::Object& options = nullptr, ui::app* app = nullptr, bool deferred = false) {
				return get_factory().template create<T>(parent ? parent : &view(), options, app, deferred);
			}

		private:
			utils::ui::node_ptr m_view;
		};
		using view_controller_ptr = std::shared_ptr<view_controller>;
	}
}