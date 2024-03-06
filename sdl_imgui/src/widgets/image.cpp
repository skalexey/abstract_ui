#include <filesystem>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <imgui.h>
#include <SDL3/SDL.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>
#include <utils/Log.h>
#include <abstract_ui/imgui/sdl_app.h>
#include <abstract_ui/imgui/widget_factory.h>
#include <abstract_ui/imgui/widgets/image.h>

LOG_TITLE("imgui::image");

namespace
{
	REGISTER_WIDGET_DECLARATIVELY(utils::ui::imgui::widget_factory, utils::ui::imgui::image);

	SDL_Surface* CreateRGBSurfaceFrom(void* pixels, int width, int height, int depth, int pitch, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
	{
		return SDL_CreateSurfaceFrom(pixels, width, height, pitch,
			SDL_GetPixelFormatEnumForMasks(depth, Rmask, Gmask, Bmask, Amask));
	}

	bool load_texture_from_file(const std::string& fpath, SDL_Texture** texture_ptr, int& width, int& height, SDL_Renderer* renderer)
	{
		int channels;
		unsigned char *data = stbi_load(fpath.c_str(), &width, &height, &channels, 0);
		if (data == nullptr)
		{
			LOG_ERROR("Failed to load image '" << fpath << "'. Reason: " << stbi_failure_reason());
			return false;
		}
		SDL_Surface *surface = CreateRGBSurfaceFrom((void *)data, width, height, channels * 8, channels * width,
														0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
		if (surface == nullptr)
		{
			LOG_ERROR("Failed to create SDL surface: " << SDL_GetError());
			return false;
		}
		*texture_ptr = SDL_CreateTextureFromSurface(renderer, surface);
		if ((*texture_ptr) == nullptr)
		{
			LOG_ERROR("Failed to create SDL texture: " << SDL_GetError());
		}
		SDL_DestroySurface(surface);
		stbi_image_free(data);
		return true;
	}

	struct texture_info_t
	{
		SDL_Texture* texture = nullptr;
		int width = 0;
		int height = 0;
		
		~texture_info_t()
		{
			if (texture)
				SDL_DestroyTexture(texture);
		}
	};

	class texture_manager
	{
	public:
		texture_manager(utils::ui::imgui::sdl_app* app)
			: m_app(app)
			, m_textures(m_data[m_app])
		{}

		texture_info_t* texture_info(const std::string& path)
		{
			auto it = m_textures.find(path);
			if (it != m_textures.end())
				return &it->second;
			return load_texture(path);
		}

	private:
		texture_info_t* load_texture(const std::string& path)
		{
			auto& texture_info = m_textures[path];
			if (load_texture_from_file(path, &texture_info.texture, texture_info.width, texture_info.height, m_app->renderer()))
				return &texture_info;
			return nullptr;
		}
	
	private:
		utils::ui::imgui::sdl_app* m_app = nullptr;
		std::unordered_map<std::string, texture_info_t>& m_textures;
		static std::unordered_map<utils::ui::imgui::sdl_app*, std::unordered_map<std::string, texture_info_t>> m_data;
	};

	std::unordered_map<utils::ui::imgui::sdl_app*, std::unordered_map<std::string, texture_info_t>> texture_manager::m_data;
}

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			int imgui::image::on_post_construct()
			{
				if (!get_texture().empty())
					init_texture();
				return 0;
			}

			void imgui::image::on_set_texture()
			{
				init_texture();
			}

			int imgui::image::init_texture()
			{
				auto path = std::filesystem::path("assets/textures/" + get_texture());
				auto& size = get_size();
				if (auto texture_info = texture_manager(&app()).texture_info(path.string()))
				{
					m_texture = texture_info->texture;
					set_size({ texture_info->width, texture_info->height });
					return 0;
				}
				return -1;
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
				ImGui::Image((void*)m_texture, ImVec2(size.x, size.y));
				return true;
			}
		}
	}
}