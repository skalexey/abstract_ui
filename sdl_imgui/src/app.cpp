#include <chrono>
#include <imgui.h>
#include <abstract_ui/imgui/app.h>
#include <utils/file_utils.h>
#ifdef ANDROID
#include "bridge.h"
#endif
#include <abstract_ui/imgui/register_widgets.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			int app::on_run()
			{
				imgui::register_widgets();

				//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
				//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

				// Use method init() here or something like this to call this code:
				// IMGUI_CHECKVERSION();
				// ImGui::CreateContext();
				// ----------------------------------------------------------------
				// Setup Dear ImGui style
				ImGui::StyleColorsDark();
				//ImGui::StyleColorsLight();

				ImGuiIO& io = ImGui::GetIO(); (void)io;

				// Load Fonts
				// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
				// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
				// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
				// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
				// - Read 'docs/FONTS.md' for more instructions and details.
				// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
				//io.Fonts->AddFontDefault();
				//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
				//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
				//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
				//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
#ifdef ANDROID
				auto resources_dir = android_get_temp_dir();
				auto fpath = resources_dir + "/arial.ttf";
				if (!utils::file::exists(fpath))
					android_copy_assets();
				ImFont* font = io.Fonts->AddFontFromFileTTF((resources_dir + "/arial.ttf").c_str(), 18.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
#elif defined(__linux__)
				ImFont* font = io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/open-sans/OpenSans-Regular.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
#elif defined(_WIN32)
				ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\Arial.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
#elif defined(__APPLE__)
				ImFont* font = io.Fonts->AddFontFromFileTTF("/Library/Fonts/Arial Unicode.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
#endif
				IM_ASSERT(font != NULL);
				//io.Fonts->AddFontFromFileTTF("NotoSansCJKjp-Medium.otf", 20.0f, NULL, io.Fonts->GetGlyphRangesJapanese());


				// io.FontGlobalScale = 1.5f;
				// ImGuiStyle& style = ImGui::GetStyle();
				// style.ScaleAllSizes(1.5f);
				// Initial code
				auto ret = init();

				// Main loop
				if (ret == 0)
				{
					using namespace std::chrono;
					long long ns = 0;
					auto start = steady_clock::now();
					do
					{
						auto end = steady_clock::now();
						ns = duration_cast<nanoseconds>(end - start).count();
						start = end;
					} while (update(float(ns) * float(1e-9)));
				}

				// Cleanup
				cleanup();
				return ret;
			}

			void app::cleanup()
			{
				ImGui::DestroyContext();
			}

			bool app::update(float dt)
			{
				ImGui::NewFrame();
				bool active = base::update(dt);
				ImGui::Render();
				return active;
			}
		}
	}
}
