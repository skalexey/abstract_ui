
#pragma once
#include <cassert>
#include <memory>
#include <abstract_ui/fwd.h>
#include <abstract_ui/node.h>

namespace utils
{
	namespace ui
	{
		class view_controller : public node
		{
		public:
			using base = node;
			~view_controller() override {
				m_view->remove_from_parent();
			}
			
		protected:
			template <typename T>
			void set_view() {
				m_view = std::dynamic_pointer_cast<utils::ui::node>(get_factory().create_final<T>(*parent(), get_options()));
			}
			const utils::ui::node_ptr& get_view() {
				return m_view;
			}

		private:
			utils::ui::node_ptr m_view;
		};
		using view_controller_ptr = std::shared_ptr<view_controller>;
	}
}