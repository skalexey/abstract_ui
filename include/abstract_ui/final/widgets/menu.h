#pragma once

#include <abstract_ui/fwd.h>
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

				void on_set_back_button_enabled() override {
					if (auto impl = this->impl())
						impl->set_back_button_enabled(is_back_button_enabled());
					
				}

			protected:
				// Don't create any widgets in the final node
				void create_back_button(ui::node* parent = nullptr) override {
					impl()->create_back_button(parent);
				}
				void remove_back_button() override {
					impl()->remove_back_button();
				}
				utils::ui::button_ptr get_back_button_ptr() const override {
					return impl()->get_back_button_ptr();
				}

				void on_set_impl() override {
					base::on_set_impl();
					on_set_back_button_enabled();
					on_set_title();
				}
				
				void on_set_title() override {
					if (auto impl = this->impl())
						impl->set_title(get_title());
				}
				ui::menu_ptr impl() const {
					return std::dynamic_pointer_cast<ui::menu>(final::window::impl());
				}
			};
		}
	}
}
