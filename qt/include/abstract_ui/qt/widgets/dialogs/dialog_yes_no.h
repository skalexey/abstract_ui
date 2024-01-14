#pragma once

#include <optional>
#include <abstract_ui/widgets/dialogs/dialog_yes_no.h>
#include <abstract_ui/qt/widgets/dialog.h>
#include <utils/common_macros.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class dialog_yes_no : public qt::dialog, public ui::dialog_yes_no
			{
			public:
				using base = ui::dialog_yes_no;
				
				void construct(
					const std::string& msg
					, const on_answer_t& on_answer = {}
					, const char* yes_text = nullptr
					, const char* no_text = nullptr
					, const std::optional<std::string>& title = {}
				)  override;

				void on_show() override {
					qt::dialog::on_show();
				}

			protected:
				void button_yes_show() override;
			
			private:
				int init() override {
					RETURN_IF_NE_0(qt::dialog::init());
					return base::init();
				}
			};
		}
	}
}