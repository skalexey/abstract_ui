#pragma once

#include <abstract_ui/fwd.h>
#include <abstract_ui/window.h>
#include <abstract_ui/final/window.h>
#include <abstract_ui/widgets/dialog.h>

namespace utils
{
	namespace ui
	{
		namespace final
		{
			class dialog : public virtual ui::dialog, public final::window
			{
			public:
				using impl_t = ui::dialog;
				using base = final::window;

				dialog(const ui::dialog_ptr& impl) : base(impl) {}
			
			protected:
				void on_set_title() override {
					impl()->set_title(get_title());
				}

				ui::dialog_ptr impl() const {
					return std::dynamic_pointer_cast<ui::dialog>(final::window::impl());
				}
			};
		}
	}
}
