
#pragma once
#include <cassert>
#include <memory>
#include <abstract_ui/fwd.h>
#include <abstract_ui/node.h>

namespace utils
{
	namespace ui
	{
		class window_controller : public node
		{
		public:
			window_controller();
			
		protected:
			void set_view(const utils::ui::window_ptr& view) {
				m_view = view;
			}
			const utils::ui::window_ptr& get_view() {
				return m_view;
			}

		private:
			utils::ui::window_ptr m_view;
		};
		using window_controller_ptr = std::shared_ptr<window_controller>;
	}
}