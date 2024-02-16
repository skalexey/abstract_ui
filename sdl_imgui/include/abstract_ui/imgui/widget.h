#pragma once

#include <memory>
#include <abstract_ui/imgui/node.h>
#include <abstract_ui/widget.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class widget : public virtual imgui::node, public virtual ui::widget
			{
			public:
				using base = ui::widget;

				vec2i get_screen_size() const override;
				void set_size(const vec2i& size) override {
					bool size_changed = size != m_size;
					m_size = size;
					on_set_size();
					if (size_changed)
						on_size_changed();
				}
				void set_max_width(int value) override {
					// TODO: implement
				}
				int get_max_width() const override {
					// TODO: implement
					return -1;
				}
			};

			using widget_ptr = std::shared_ptr<imgui::widget>;
		}
	}
}