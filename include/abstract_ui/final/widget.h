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

				widget(const ui::widget_ptr& impl) : base(impl) {}

				// Virtual methods redirections
				void set_on_show(const on_show_cb& on_show) override {
					impl()->set_on_show(on_show);
				}
				void set_size(const vec2i& size) override {
					impl()->set_size(size);
				}
				void set_size_relative(const vec2f& size) override {
					impl()->set_size_relative(size);
				}
				
			protected:
				ui::widget_ptr impl() const {
					return std::dynamic_pointer_cast<ui::widget>(final::node::impl());
				}
			};
		}
	}
}
