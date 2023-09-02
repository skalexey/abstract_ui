#pragma once

#include <string>
#include <abstract_ui/qt/fwd.h>
#include <abstract_ui/qt/widgets/text_input_model.h>
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
				const QQuickItem* get_text_field() const;
				const QQuickItem* get_label() const;
				QQuickItem* text_field();
				QQuickItem* label();

			protected:
				int init() override;
				void on_set_on_new_value() override;

			private:
				text_input_model* m_model = nullptr;
				mutable std::string m_tmp_value; // Needed as get_value() returns const std::string&,
				// but Qt can only return std::string created from QString which in turn is created from QVariant.
				QQuickItem* m_text_field = nullptr;
				QQuickItem* m_label = nullptr;
			};
			using text_input_ptr = std::shared_ptr<text_input>;
		}
	}
}