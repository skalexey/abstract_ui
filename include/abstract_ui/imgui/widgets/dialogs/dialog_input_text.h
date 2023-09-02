#pragma once

#include <memory>
#include <string>
#include <abstract_ui/imgui/fwd.h>
#include <abstract_ui/widgets/dialogs/dialog_input_text.h>
#include <abstract_ui/imgui/widgets/dialog.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			// Order of inheritance is important here as imgui::dialog sets the widget factory first
			class dialog_input_text : public imgui::dialog, public ui::dialog_input_text
			{
			public:
				using base = ui::dialog_input_text;
				
				void on_show() override {
					imgui::dialog::on_show();
				}
			};
		}
	}
}