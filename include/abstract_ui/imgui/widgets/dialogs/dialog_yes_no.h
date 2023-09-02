#pragma once

#include <optional>
#include <abstract_ui/widgets/dialogs/dialog_yes_no.h>
#include <abstract_ui/imgui/widgets/dialog.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			// Order of inheritance is important here as imgui::dialog sets the widget factory first
			class dialog_yes_no : public imgui::dialog, public ui::dialog_yes_no
			{
			public:
				using base = ui::dialog_yes_no;
				
				dialog_yes_no();

				void construct(
					const std::string& msg
					, const on_answer_t& on_answer = {}
					, const char* yes_text = nullptr
					, const char* no_text = nullptr
					, const std::optional<std::string>& title = {}
				) override;

				void on_show() override {
					imgui::dialog::on_show();
				}

			protected:
				void button_yes_show() override;
			
			private:
				int this_on_post_construct();
			};
		}
	}
}