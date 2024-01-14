#pragma once

#include <memory>
#include <abstract_ui/qt/widget_factory.h>
#include <abstract_ui/widget.h>
#include <abstract_ui/qt/widgets/button.h>
#include <abstract_ui/qt/widgets/dialog.h>
#include <abstract_ui/qt/widgets/label.h>
#include <abstract_ui/qt/widgets/text.h>
#include <abstract_ui/qt/widgets/text_input.h>
#include <abstract_ui/qt/widgets/dialogs/dialog_input_text.h>
#include <abstract_ui/qt/widgets/dialogs/dialog_message.h>
#include <abstract_ui/qt/widgets/dialogs/dialog_with_buttons.h>
#include <abstract_ui/qt/widgets/dialogs/dialog_yes_no.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			inline void register_widgets()
			{
				REGISTER_WIDGET(qt::widget_factory, qt::button);
				REGISTER_WIDGET(qt::widget_factory, qt::dialog);
				REGISTER_WIDGET(qt::widget_factory, qt::label);
				REGISTER_WIDGET(qt::widget_factory, qt::text);
				REGISTER_WIDGET(qt::widget_factory, qt::text_input);
				REGISTER_WIDGET(qt::widget_factory, qt::dialog_input_text);
				REGISTER_WIDGET(qt::widget_factory, qt::dialog_message);
				REGISTER_WIDGET(qt::widget_factory, qt::dialog_with_buttons);
				REGISTER_WIDGET(qt::widget_factory, qt::dialog_yes_no);
			}
		}
	}
}