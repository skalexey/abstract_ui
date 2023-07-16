
#pragma once
#include <cassert>
#include <memory>
#include <abstract_ui/fwd.h>

namespace utils
{
	namespace ui
	{
		class window_controller
		{
		public:
			window_controller(utils::ui::app& app) : m_app(app) {}
			virtual bool show();
			const widget_factory& get_factory() const;

			bool update(float dt) {
				return true;
			}
			
		protected:
			void set_view(const utils::ui::window_ptr& view) {
				m_view = view;
			}
			const utils::ui::window_ptr& get_view() {
				return m_view;
			}

		private:
			utils::ui::window_ptr m_view;
			utils::ui::app& m_app;
		};
		using window_controller_ptr = std::shared_ptr<window_controller>;
	}
}