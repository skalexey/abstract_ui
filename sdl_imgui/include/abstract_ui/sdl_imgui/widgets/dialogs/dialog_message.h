#pragma once

#include <string>
#include <abstract_ui/widgets/dialogs/dialog_message.h>
#include <abstract_ui/imgui/widgets/dialog.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			// Order of inheritance is important here as imgui::dialog sets the widget factory first
			class dialog_message : public imgui::dialog, public ui::dialog_message
			{
			public:
				using base = utils::ui::dialog_message;
				dialog_message();
				
			private:
				WIDGET_REGISTRATOR(imgui::widget_factory, dialog_message);
			};
		}
	}
}