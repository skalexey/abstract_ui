#pragma once

#include <memory>
#include <abstract_ui/imgui/fwd.h>
#include <abstract_ui/widgets/menu_item.h>
#include <abstract_ui/imgui/widget.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class menu_item : public imgui::widget, public ui::menu_item
			{
			public:
				using base = ui::menu_item;
				
				void set_enabled(bool enabled) override {
					m_enabled = enabled;
				}
				bool is_enabled() const override {
					return m_enabled;
				}

			protected:
				virtual bool on_imgui_menu_item_update(float dt) {
					return true;
				}

			private:
				bool imgui_menu_item_update(float dt);
				bool on_update(float dt) override final;

			private:
				bool m_enabled = true;
			};
			using menu_item_ptr = std::shared_ptr<menu_item>;
		}
	}
}