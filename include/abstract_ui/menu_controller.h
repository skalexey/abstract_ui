
#pragma once
#include <cassert>
#include <memory>
#include <abstract_ui/fwd.h>
#include <abstract_ui/widgets/menu.h>
#include <abstract_ui/view_controller.h>

namespace utils
{
	namespace ui
	{
		class menu_controller : public view_controller
		{
		public:
			using base = view_controller;
			void close();

		protected:
			const utils::ui::menu& get_view() const {
				return dynamic_cast<const utils::ui::menu&>(base::get_view());
			}
			utils::ui::menu& view() {
				return const_cast<utils::ui::menu&>(get_view());
			}
		};
		using menu_controller_ptr = std::shared_ptr<menu_controller>;
	}
}