#pragma once

#include <abstract_ui/widgets/dialog.h>
#include <abstract_ui/imgui/window.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class dialog : public imgui::window, public virtual ui::dialog
			{
			public:
				using base = ui::dialog;

			public:
				// We don't pass the title as argument because of virtual inheritance
				dialog();
				
			protected:
				bool update_children(float dt) override final;
				
			private:
				vec2i m_last_size;
				vec2i m_last_position;
			};
			using dialog_ptr = std::shared_ptr<dialog>;
		}
	}
}