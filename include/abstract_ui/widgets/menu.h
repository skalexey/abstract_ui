#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <abstract_ui/window.h>
#include <abstract_ui/fwd.h>

namespace utils
{
	namespace ui
	{
		class menu : public virtual window
		{
		public:
			using base = window;
			
			menu() {
				// Don't override on_post_construct to keep it free to use for derived classes
				do_on_post_construct([self = this]() {
					self->set_title("qt menu");
					auto screen_size = self->get_screen_size();
					self->set_size(screen_size);
					return 0;
				});
			}
			const std::string& get_title() const override {
				return m_title;
			}
			void set_title(const std::string& title) {
				m_title = title;
				on_set_title();
			}
			
			bool is_back_button_enabled() const { return m_use_back_button; }
			void set_back_button_enabled(bool enabled) { m_use_back_button = enabled; }

		protected:	
			virtual void on_set_title() {}

		private:
			std::string m_title = "menu";
			bool m_use_back_button = false;
		};
		using menu_ptr = std::shared_ptr<menu>;
	}
}