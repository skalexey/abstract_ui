#pragma once

#include <optional>
#include <abstract_ui/widgets/dialogs/dialog_with_buttons.h>
#include <abstract_ui/qt/widgets/dialog.h>
#include <utils/common_macros.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			// Order of inheritance is important here as qt::dialog sets the widget factory first
			class dialog_with_buttons: public qt::dialog, public ui::dialog_with_buttons
			{
			public:
				using base = ui::dialog_with_buttons;
				
				// In place of a constructor as we only support default one
				void construct(
				    const std::string& msg
					, const actions_t& actions = {}
					, const std::optional<std::string>& title = {}
				)
				{
					base::construct(msg, actions, title);
				}

				void on_show() override {
					utils::ui::qt::dialog::on_show();
				}

			private:
				int init() override {
					RETURN_IF_NE_0(qt::dialog::init());
					return ui::dialog_with_buttons::init();
				}
				WIDGET_REGISTRATOR(qt::widget_factory, dialog_with_buttons);
			};
		}
	}
}