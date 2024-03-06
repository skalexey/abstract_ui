// 
// ImGui-SDL Application UI logic controller.

#pragma once

#include <abstract_ui/imgui/app.h>

struct SDL_Renderer;
struct SDL_Window;

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class sdl_app : public imgui::app
			{
				friend class imgui::sdl_node;

			public:
				using base = imgui::app;

				sdl_app(int argc, char* argv[])
					: ui::app(argc, argv)
					, base::app(argc, argv)
				{}
				bool update(float dt) override final;
				virtual SDL_Window* create_window();
				SDL_Window* window() {
					return m_window;
				}
				SDL_Renderer* renderer() {
					return m_renderer;
				}

			public:
				static void request_keyboard();
				
			protected:
				int on_run() override final;
				void cleanup() override final;
				
			private:
				SDL_Renderer* m_renderer = nullptr;
				SDL_Window* m_window = nullptr;
			};
		}
	}
}
