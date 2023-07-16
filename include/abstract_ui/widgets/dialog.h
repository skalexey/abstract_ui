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
		class dialog : public virtual window
		{
			using base = window;

		public:
			dialog()
			// We don't pass a factory into the constructor because of virtual inheritance,
			// so it should be set using set_factory() method
			{
				// Default size
				set_size(vec2i(500, 320));
				set_vertical_alignment(alignment::center);
				set_horizontal_alignment(alignment::center);
			}

			const std::string& get_title() const override {
				return m_title;
			}
			void set_title(const std::string& title) {
				m_title = title;
				on_set_title();
			}
			
			bool is_close_button_enabled() const { return m_use_close_button; }
			bool set_close_button_enabled(bool enabled) { m_use_close_button = enabled; }

			bool is_modal() const { return m_is_modal; }
			void set_modal(bool modal) { m_is_modal = modal; }

			bool is_auto_resize() const { return m_auto_resize; }
			void set_auto_resize(bool auto_resize) { m_auto_resize = auto_resize; }

		protected:	
			virtual void on_set_title() {}
			virtual void on_set_modal() {}
			virtual bool on_dialog_update(float dt) {
				return true;
			}

			bool dialog_update(float dt) {
				if (get_horizontal_alignment() == alignment::center && get_vertical_alignment() == alignment::center) {
					set_position_relative({ 0.5f, 0.5f }, { 0.5f, 0.5f });
				}
				return true;
			}

		//private:
			bool on_update(float dt) override { // Not final for virtual inheritance
				if (!dialog_update(dt))
					return false;
				if (!base::on_update(dt))
					return false;
				return on_dialog_update(dt);
			};

		private:
			std::string m_title = "Dialog";
			bool m_use_close_button = false;
			bool m_is_modal = false;
			bool m_auto_resize = false;
		};
		using dialog_ptr = std::shared_ptr<dialog>;
	}
}