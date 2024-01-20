#pragma once

#include <memory>
#include <abstract_ui/qt/fwd.h>
#include <abstract_ui/widgets/button.h>
#include <abstract_ui/qt/widget.h>

class QQuickButton;

namespace utils
{
	namespace ui
	{
		namespace qt
        {
			class button : public qt::widget, public ui::button
			{
			public:
				using base = ui::button;

				const vec2i& text_size();

				const vec2i& get_size() {
					return m_calculated_size;
				}

				void on_set_text(const std::string& text) override;
				void on_set_on_click(const on_click_t& on_click) override;

				void set_enabled(bool enabled) override;
				button_model* model() {
					return const_cast<button_model*>(get_model());
				}
				const button_model* get_model() const;

			protected:
				int init() override;
				widget_model* create_model() const override;

			private:
				vec2i m_calculated_size;
			};
			using button_ptr = std::shared_ptr<button>;
		}
	}
}
