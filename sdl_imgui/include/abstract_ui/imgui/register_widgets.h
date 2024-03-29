#pragma once

#include <memory>
#include <abstract_ui/imgui/widget_factory.h>
#include <abstract_ui/widget.h>
#include <abstract_ui/imgui/widgets/layouts/vertical_layout.h>
#include <abstract_ui/imgui/widgets/layouts/horizontal_layout.h>
#include <abstract_ui/imgui/widgets/layouts/auto_layout.h>
#include <abstract_ui/imgui/widgets/button.h>
#include <abstract_ui/imgui/widgets/dialog.h>
#include <abstract_ui/imgui/widgets/menu.h>
#include <abstract_ui/imgui/widgets/menu_item.h>
#include <abstract_ui/imgui/widgets/menu_list.h>
#include <abstract_ui/imgui/widgets/menu_bar.h>
#include <abstract_ui/imgui/widgets/label.h>
#include <abstract_ui/imgui/widgets/text.h>
#include <abstract_ui/imgui/widgets/text_input.h>
#include <abstract_ui/imgui/widgets/image.h>
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
				REGISTER_WIDGET(imgui::widget_factory, imgui::widget);
				REGISTER_WIDGET(imgui::widget_factory, imgui::vertical_layout);
				REGISTER_WIDGET(imgui::widget_factory, imgui::horizontal_layout);
				REGISTER_WIDGET(imgui::widget_factory, imgui::auto_layout);
				REGISTER_WIDGET(imgui::widget_factory, imgui::button);
				REGISTER_WIDGET(imgui::widget_factory, imgui::dialog);
				REGISTER_WIDGET(imgui::widget_factory, imgui::menu);
				REGISTER_WIDGET(imgui::widget_factory, imgui::menu_item);
				REGISTER_WIDGET(imgui::widget_factory, imgui::menu_list);
				REGISTER_WIDGET(imgui::widget_factory, imgui::menu_bar);
				REGISTER_WIDGET(imgui::widget_factory, imgui::label);
				REGISTER_WIDGET(imgui::widget_factory, imgui::text);
				REGISTER_WIDGET(imgui::widget_factory, imgui::text_input);
				REGISTER_WIDGET(imgui::widget_factory, imgui::image);
				REGISTER_WIDGET(imgui::widget_factory, imgui::dialog_input_text);
				REGISTER_WIDGET(imgui::widget_factory, imgui::dialog_message);
				REGISTER_WIDGET(imgui::widget_factory, imgui::dialog_with_buttons);
				REGISTER_WIDGET(imgui::widget_factory, imgui::dialog_yes_no);
			}
		}
	}
}