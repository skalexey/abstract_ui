#pragma once

#include <abstract_ui/fwd.h>
#include <abstract_ui/window.h>
#include <abstract_ui/final/widget.h>

namespace utils
{
	namespace ui
	{
		namespace final
		{
			class window : public virtual ui::window, public final::widget
			{
			public:
				using impl_t = ui::window;
				using base = final::widget;

				window(const ui::window_ptr& impl) : base(impl) {}

				// Pure virtual methods specifications
				vec2i get_screen_size() const override {
					return get_impl()->get_screen_size();
				}

			protected:
				ui::window_ptr impl() {
					return std::dynamic_pointer_cast<ui::window>(final::widget::impl());
				}

				const ui::window_ptr& get_impl() const {
					return std::dynamic_pointer_cast<ui::window>(final::widget::get_impl());
				}
			};
		}
	}
}
