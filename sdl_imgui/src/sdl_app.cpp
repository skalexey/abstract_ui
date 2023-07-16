#include <SDL.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer.h>
#include <abstract_ui/imgui/sdl_app.h>
#include <utils/file_utils.h>
#include <abstract_ui/imgui/widget_factory.h>
#ifdef ANDROID
#include "bridge.h"
#endif
#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

int g_keyboard_request_count = 0;

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			int sdl_app::on_run()
			{
				add_on_update([this](float dt) {
					if (SDL_HasScreenKeyboardSupport())
					{
						if (g_keyboard_request_count > 0)
						{
							if (!SDL_IsScreenKeyboardShown(get_window()))
							{
								// SDL_Rect r = { m_resolution.x + 60, m_resolution.y + 30, m_resolution.x - 180, m_resolution.y - 60 };
								SDL_Rect r = { 0, 0, 0, 0 };
								SDL_SetTextInputRect(&r);
								SDL_StartTextInput();
							}
						}
						else
							if (SDL_IsScreenKeyboardShown(get_window()))
								SDL_StopTextInput();
					}
					return true;
				});
				// Setup SDL
				// (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
				// depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
				
				if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
				{
					printf("Error: %s\n", SDL_GetError());
					return -1;
				}

				SDL_DisplayMode DM;
				SDL_GetCurrentDisplayMode(0, &DM);
				auto sw = DM.w;
				auto sh = DM.h;
				SDL_Log("Display: %dx%d", sw, sh);

				get_resolution().y = get_resolution().x / (sw / sh);
				m_window = create_window();

				// Setup SDL_Renderer instance
				m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
				if (m_renderer == NULL)
				{
					SDL_Log("Error creating SDL_Renderer!");
					return -2;
				}

				SDL_RenderSetLogicalSize(m_renderer, get_resolution().x, get_resolution().y);

				//SDL_RendererInfo info;
				//SDL_GetRendererInfo(renderer, &info);
				//SDL_Log("Current SDL_Renderer: %s", info.name);

				// Setup Platform/Renderer backends

				// Setup Dear ImGui context
				// TODO: move this to imgui::app (or maybe not? :thinking:)
				IMGUI_CHECKVERSION();
				ImGui::CreateContext();
				// ----------------------------------------------------------
				ImGui_ImplSDL2_InitForSDLRenderer(m_window, m_renderer);
				ImGui_ImplSDLRenderer_Init(m_renderer);

				return base::on_run();
			}

			void sdl_app::cleanup()
			{
				ImGui_ImplSDLRenderer_Shutdown();
				ImGui_ImplSDL2_Shutdown();
				base::cleanup();
				SDL_DestroyRenderer(m_renderer);
				SDL_DestroyWindow(m_window);
				SDL_Quit();
			}

			bool sdl_app::update(float dt)
			{
				assert(m_renderer);
				// Every widget can request keyboard input by calling request_keyboard()
				// in every frame which increments this counter
				g_keyboard_request_count = 0;

				// Poll and handle events (inputs, window resize, etc.)
				// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
				// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
				// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
				// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
				SDL_Event event;
				bool active = true;
				while (SDL_PollEvent(&event))
				{
					auto& io = ImGui::GetIO();
#ifdef ANDROID
					if (event.type != SDL_TEXTINPUT)
#endif
						ImGui_ImplSDL2_ProcessEvent(&event);
					switch (event.type)
					{
						case SDL_TEXTINPUT:
							SDL_Log("SDL_TEXTINPUT: '%s'", event.text.text);
						break;

						case SDL_TEXTEDITING:
							SDL_Log("SDL_TEXTEDITING: '%s' [%d, %d]", event.edit.text, event.edit.start, event.edit.length);
							if (auto state = ImGui::GetInputTextState(ImGui::GetActiveID()))
								state->ClearText();
							io.AddInputCharactersUTF8(event.text.text);
						break;

						case SDL_QUIT:
							active = false;
						break;

						case SDL_WINDOWEVENT:
							if (event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(m_window))
								active = false;
						break;
					}
				}

				// Start the Dear ImGui frame
				ImGui_ImplSDLRenderer_NewFrame();
				ImGui_ImplSDL2_NewFrame();

				if (active)
					active = base::update(dt);

				// Rendering
				SDL_SetRenderDrawColor(m_renderer, (Uint8)(m_clear_color.x * 255), (Uint8)(m_clear_color.y * 255), (Uint8)(m_clear_color.z * 255), (Uint8)(m_clear_color.w * 255));
				SDL_RenderClear(m_renderer);
				ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
				SDL_RenderPresent(m_renderer);
					
				return active;
			}

			SDL_Window* ui::imgui::sdl_app::create_window()
			{
				SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
				return SDL_CreateWindow("ImGui SDL Application", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, get_resolution().x, get_resolution().y, window_flags);
			}

			void ui::imgui::sdl_app::request_keyboard()
			{
				g_keyboard_request_count++;
			}
		}
	}
}
