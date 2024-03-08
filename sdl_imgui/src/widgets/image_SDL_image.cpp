/*
// *** EXPERIMENTAL ***
#include <filesystem>
#include <string>
#include <SDL3/SDL.h>
// #include <SDL_image.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>
#include <utils/log.h>
#include <abstract_ui/imgui/widget_factory.h>
#include <abstract_ui/imgui/widgets/image.h>

LOG_TITLE("imgui::image");

namespace
{
	REGISTER_WIDGET_DECLARATIVELY(utils::ui::imgui::widget_factory, utils::ui::imgui::image);

	//GLuint LoadTextureFromFile(const std::string& filename) {
	//	// Load image using SDL_image
	//	SDL_Surface* surface = IMG_Load(filename);
	//	if (!surface) {
	//		LOG_ERROR("Failed to load image: " << SDL_GetError());
	//		return 0;
	//	}

	//	// Create OpenGL texture
	//	GLuint texture_id;
	//	glGenTextures(1, &texture_id);
	//	glBindTexture(GL_TEXTURE_2D, texture_id);
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//	SDL_FreeSurface(surface);
	//	return texture_id;
	//}
}

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			int imgui::image::on_post_construct()
			{
				auto path = std::filesystem::path("textures/" + get_texture());
				//m_texture_id = LoadTextureFromFile(path.string();
				return m_texture_id ? 0 : -1;
			}

			bool imgui::image::on_update(float dt)
			{
				if (!imgui_image_update(dt))
					return false;
				return imgui::widget::on_update(dt);
			}

			bool imgui::image::imgui_image_update(float dt)
			{
				auto& size = get_size();
				// ImGui::Image((void*)(intptr_t)m_texture_id, ImVec2(size.x, size.y));
				return true;
			}
		}
	}
}
*/