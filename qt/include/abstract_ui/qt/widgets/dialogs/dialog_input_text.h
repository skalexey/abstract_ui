#pragma once

#include <memory>
#include <string>
#include <abstract_ui/qt/fwd.h>
#include <abstract_ui/widgets/dialogs/dialog_input_text.h>
#include <abstract_ui/qt/widgets/dialog.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			// Order of inheritance is important here as qt::dialog sets the widget factory first
			class dialog_input_text : public qt::dialog, public ui::dialog_input_text
			{
			public:
				using base = ui::dialog_input_text;
				
				void on_show() override {
					qt::dialog::on_show();
				}
			};
		}
	}
}