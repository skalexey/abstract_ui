#pragma once

#include <abstract_ui/fwd.h>
#include <abstract_ui/final/widget.h>
#include <abstract_ui/widgets/menu_list_base.h>

namespace utils
{
	namespace ui
	{
		namespace final
		{
			class menu_list_base : public virtual ui::menu_list_base, public final::widget
			{
			public:
				using impl_t = ui::menu_list_base;
				using base = final::widget;

				menu_list_base() = default;
				menu_list_base(const ui::menu_list_base_ptr& impl) : base(impl) {}
				void add_widget(const ui::widget_ptr& widget) override {
					impl()->add_widget(widget);
				}
			protected:
				ui::menu_list_base_ptr impl() const {
					return std::dynamic_pointer_cast<ui::menu_list_base>(final::widget::impl());
				}
			};
		}
	}
}
