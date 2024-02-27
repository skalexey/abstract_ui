#pragma once

#include <memory>
#include <QVariantMap>
#include <abstract_ui/qt/fwd.h>
#include <abstract_ui/widgets/clickable_item.h>
#include <abstract_ui/qt/widget.h>

class QUrl;

namespace utils
{
	namespace ui
	{
		namespace qt
        {
			class clickable_item : public virtual qt::widget, public virtual ui::clickable_item
			{
			public:
				using base = ui::clickable_item;

				const vec2i& text_size();

				const vec2i& get_size() {
					return m_calculated_size;
				}

				void set_enabled(bool enabled) override;
				bool is_enabled() const override;
				
				clickable_item_model* model() {
					return const_cast<clickable_item_model*>(get_model());
				}
				const clickable_item_model* get_model() const;

			protected:
				void on_set_text() override;
				void on_set_on_click(const on_click_t& on_click) override;
				int init(const QUrl& url, const QVariantMap& initial_properties = {}) override;
				widget_model* create_model() const override;

			private:
				vec2i m_calculated_size;
			};
			using clickable_item_ptr = std::shared_ptr<clickable_item>;
		}
	}
}
