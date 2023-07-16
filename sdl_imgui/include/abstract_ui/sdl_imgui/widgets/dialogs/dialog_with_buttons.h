#pragma once

#include <optional>
#include <abstract_ui/widgets/dialogs/dialog_with_buttons.h>
#include <abstract_ui/imgui/widgets/dialog.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			// Order of inheritance is important here as imgui::dialog sets the widget factory first
			class dialog_with_buttons: public imgui::dialog, public ui::dialog_with_buttons
			{
			public:
				using base = ui::dialog_with_buttons;
				
				void on_show() override {
					imgui::dialog::on_show();
				}

			private:
				WIDGET_REGISTRATOR(imgui::widget_factory, dialog_with_buttons);
			};
		}
	}
}