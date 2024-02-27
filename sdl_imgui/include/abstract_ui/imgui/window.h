// TODO: Do we really need this class?
#pragma once

#include <memory>
#include <imgui.h>
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
				void on_add_node(ui::node* node) override;
				virtual ImGuiWindowFlags window_flags() const {
					return m_window_flags;
				}
				virtual bool* p_open() {
					return &m_open;
				}
			private:
				ImGuiWindowFlags m_window_flags = 0;
				bool m_open = true;
			};

			using window_ptr = std::shared_ptr<window>;
		}
	}
}