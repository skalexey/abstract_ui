#pragma once

#include <memory>
#include <abstract_ui/imgui/widget_factory.h>
#include <abstract_ui/widget.h>
#include <abstract_ui/imgui/widgets/button.h>
#include <abstract_ui/imgui/widgets/dialog.h>
#include <abstract_ui/imgui/widgets/label.h>
#include <abstract_ui/imgui/widgets/text.h>
#include <abstract_ui/imgui/widgets/text_input.h>
#include <abstract_ui/imgui/widgets/dialogs/dialog_input_text.h>
#include <abstract_ui/imgui/widgets/dialogs/dialog_message.h>
#include <abstract_ui/imgui/widgets/dialogs/dialog_with_buttons.h>
#include <abstract_ui/imgui/widgets/dialogs/dialog_yes_no.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			inline void register_widgets()
			{
				REGISTER_WIDGET(imgui::widget_factory, imgui::button);
				REGISTER_WIDGET(imgui::widget_factory, imgui::dialog);
				REGISTER_WIDGET(imgui::widget_factory, imgui::label);
				REGISTER_WIDGET(imgui::widget_factory, imgui::text);
				REGISTER_WIDGET(imgui::widget_factory, imgui::text_input);
				REGISTER_WIDGET(imgui::widget_factory, imgui::dialog_input_text);
				REGISTER_WIDGET(imgui::widget_factory, imgui::dialog_message);
				REGISTER_WIDGET(imgui::widget_factory, imgui::dialog_with_buttons);
				REGISTER_WIDGET(imgui::widget_factory, imgui::dialog_yes_no);
			}
		}
	}
}