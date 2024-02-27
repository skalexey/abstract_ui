#pragma once

#include <abstract_ui/widgets/menu.h>
#include <abstract_ui/imgui/window.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class menu : public virtual imgui::window, public virtual ui::menu
			{
			public:
				using base = ui::menu;
				
			protected:
				ImGuiWindowFlags window_flags() const override {
					auto window_flags = imgui::window::window_flags();
					window_flags |= (ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);
					return window_flags;
				}
				bool* p_open() override {
					return is_back_button_enabled() ? imgui::window::p_open() : nullptr;
				}
			};
			using menu_ptr = std::shared_ptr<menu>;
		}
	}
}