#pragma once

#include <memory>
#include <string>
#include <SDL3/SDL_render.h>
#include <abstract_ui/imgui/fwd.h>
#include <abstract_ui/widgets/image.h>
#include <abstract_ui/imgui/widget.h>
#include <abstract_ui/imgui/sdl_node.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class image : public virtual ui::imgui::sdl_node, public virtual imgui::widget, public virtual ui::image
			{
			public:
				using base = ui::image;

			protected:
				bool imgui_image_update(float dt);
				
				virtual bool on_imgui_image_update(float dt) {
					return true;
				}

				void on_set_texture() override;
				int on_post_construct() override;
				
			private:
				bool on_update(float dt) override final;
				int init_texture();

			private:
				SDL_Texture* m_texture = nullptr;
			};
			using image_ptr = std::shared_ptr<image>;
		}
	}
}