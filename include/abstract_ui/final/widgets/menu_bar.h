#pragma once

#include <abstract_ui/fwd.h>
#include <abstract_ui/final/widgets/menu_list_base.h>
#include <abstract_ui/widgets/menu_bar.h>

namespace utils
{
	namespace ui
	{
		namespace final
		{
			class menu_bar : public virtual ui::menu_bar, public virtual final::menu_list_base
			{
			public:
				using impl_t = ui::menu_bar;
				using base = final::menu_list_base;

				menu_bar() = default;
				menu_bar(const ui::menu_bar_ptr& impl) : base(impl) {}
				
			protected:
				ui::menu_bar_ptr impl() const {
					return final::menu_list_base::impl()->as_ptr<ui::menu_bar>();
				}
			};
		}
	}
}
