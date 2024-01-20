#pragma once

#include <string>
#include <abstract_ui/qt/fwd.h>
#include <abstract_ui/widgets/text_input.h>
#include <abstract_ui/qt/widget.h>

class QQuickItem;

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class text_input : public qt::widget, public utils::ui::text_input
			{
			public:
				using base = utils::ui::text_input;

				const std::string& get_value() const override;
				void set_value(const std::string& value) override;
				QQuickItem* text_field_item() const {
					return m_text_field;
				}
				QQuickItem* label_item() const{
					return m_label;
				}

				text_input_model* model() {
					return const_cast<text_input_model*>(get_model());
				}
				const text_input_model* get_model() const;
				
			protected:
				int init() override;
				widget_model* create_model() const override;
				void on_set_on_new_value() override;
				void on_set_label() override;

			private:
				mutable std::string m_tmp_value; // Needed as get_value() returns const std::string&,
				// but Qt can only return std::string created from QString which in turn is created from QVariant.
				QQuickItem* m_text_field = nullptr;
				QQuickItem* m_label = nullptr;
			};
			using text_input_ptr = std::shared_ptr<text_input>;
		}
	}
}