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
				using base = imgui::app;

			public:
				sdl_app(int argc, char* argv[])
					: ui::app(argc, argv)
					, base::app(argc, argv)
				{}
				bool update(float dt) override final;

			public:
				static void request_keyboard();
				
			protected:
				int on_run() override final;
				void cleanup() override final;
				
			protected:
				virtual SDL_Window* create_window();
				SDL_Window* get_window() { return m_window; }

			private:
				SDL_Renderer* m_renderer = nullptr;
				SDL_Window* m_window = nullptr;
			};
		}
	}
}
