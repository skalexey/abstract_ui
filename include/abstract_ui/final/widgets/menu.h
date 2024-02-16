#pragma once

#include <abstract_ui/fwd.h>
#include <abstract_ui/window.h>
#include <abstract_ui/final/window.h>
#include <abstract_ui/widgets/menu.h>

namespace utils
{
	namespace ui
	{
		namespace final
		{
			class menu : public virtual ui::menu, public final::window
			{
			public:
				using impl_t = ui::menu;
				using base = final::window;

				menu() = default;
				menu(const ui::menu_ptr& impl) : base(impl) {}

			protected:
				void on_set_title() override {
					impl()->set_title(get_title());
				}
				ui::menu_ptr impl() const {
					return std::dynamic_pointer_cast<ui::menu>(final::window::impl());
				}
			};
		}
	}
}
