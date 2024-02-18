
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
		protected:
			void set_view(const utils::ui::node_ptr& view) {
				m_view = view;
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