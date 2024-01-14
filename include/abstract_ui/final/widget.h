#pragma once

#include <abstract_ui/fwd.h>
#include <abstract_ui/final/node.h>
#include <abstract_ui/widget.h>

namespace utils
{
	namespace ui
	{
		namespace final
		{
			class widget : public virtual ui::widget, public final::node
			{
			public:
				using impl_t = ui::widget;
				using base = final::node;

				widget() = default;
				
				widget(const ui::widget_ptr& impl) : base(impl) {}

				// Virtual methods redirections
				void set_on_show(const on_show_cb& on_show) override {
					impl()->set_on_show(on_show);
				}
				void set_size(const vec2i& size) override {
					ui::widget::set_size(size);
					impl()->set_size(size);
				}
				const vec2i& get_size() const override {
					return impl()->get_size();
				}
				const vec2i& get_position() const override {
					return impl()->get_position();
				}
				void set_position(const vec2i& pos) override {
					ui::widget::set_position(pos);
					impl()->set_position(pos);
				}

			protected:
				void on_set_size_policy() override {
					impl()->set_size_policy(get_size_policy());
				}
				ui::widget_ptr impl() const {
					return std::dynamic_pointer_cast<ui::widget>(final::node::impl());
				}
			};
		}
	}
}
