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
				vec2i get_screen_size() const override;
			};

			using widget_ptr = std::shared_ptr<imgui::widget>;
		}
	}
}