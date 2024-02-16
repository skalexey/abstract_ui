
#pragma once
#include <cassert>
#include <memory>
#include <abstract_ui/fwd.h>
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
			utils::ui::menu_ptr get_view() {
				return std::dynamic_pointer_cast<utils::ui::menu>(base::get_view());
			}
		};
		using menu_controller_ptr = std::shared_ptr<menu_controller>;
	}
}