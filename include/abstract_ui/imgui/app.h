// 
// ImGui Application UI logic controller.

#pragma once

#include <imgui.h>
#include <abstract_ui/app.h>
#include <abstract_ui/imgui/node.h>
#include <utils/vec2.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class app : public virtual imgui::node, public virtual ui::app
			{
				using base = ui::app;

			public:
				app(int argc, char* argv[])
					: base::app(argc, argv)
				{}

				bool update(float dt) override;

				void set_resolution(const utils::vec2i& resolution) { m_resolution = resolution; }
				void set_resolution(int x, int y) { m_resolution = { x, y }; }
				const utils::vec2i& get_resolution() const { return m_resolution; }
			
			public:
				static void request_keyboard();
				
			protected:
				int on_run() override;
				int init() override {
					return 0;
				};

				virtual void cleanup();

				void set_clear_color(const ImVec4& color) { m_clear_color = color; }
				const ImVec4& get_clear_color() const { return m_clear_color; }
				
			protected:
				ImVec4 m_clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

			private:
				utils::vec2i m_resolution = { 1024, 720 };
			};
		}
	}
}
