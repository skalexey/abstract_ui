#include <abstract_ui/app.h>
#include <abstract_ui/widget_factory.h>
#include <abstract_ui/menu_manager.h>
#include <abstract_ui/widgets/button.h>
#include <utils/Log.h>
#include <abstract_ui/widgets/menu.h>
LOG_TITLE("menu");

namespace utils
{
	namespace ui
	{
		void ui::menu::create_back_button(ui::node* parent)
		{
			if (m_back_button)
				m_back_button->remove_from_parent();
			set_back_button_enabled(true);
			m_back_button = get_factory().create<button>(parent ? parent : this);
			if (app().menu_manager().get_menu_stack_size() > 1)
				m_back_button->set_text("Back");
			else
				m_back_button->set_text("Close");
			m_back_button->set_on_click([self = this](bool up) {
				if (up)
					self->app().menu_manager().back();
			});
		}

		void ui::menu::remove_back_button()
		{
			if (m_back_button)
			{
				m_back_button->remove_from_parent();
				m_back_button.reset();
			}
		}

		bool ui::menu::on_update(float dt)
		{
			auto back_button = get_back_button_ptr();
			if (m_use_back_button && !back_button)
				create_back_button();
			else if (!m_use_back_button && back_button)
				remove_back_button();
			return base::on_update(dt);
		}
	}
}