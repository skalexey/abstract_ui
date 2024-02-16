// TODO: Do we really need this class?
#pragma once

#include <memory>
#include <abstract_ui/window.h>
#include <abstract_ui/imgui/widget.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class window : public virtual imgui::widget, public virtual ui::window
			{
				using base = ui::window;

			protected:
				bool update_children(float dt) override;
			};

			using window_ptr = std::shared_ptr<window>;
		}
	}
}