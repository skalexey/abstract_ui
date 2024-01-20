#pragma once

#include <QObject>
#include <QString>
#include <utils/common.h>
#include <abstract_ui/qt/widget_model.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class text_input_model : public widget_model
			{
				Q_OBJECT
				using base = widget_model;
				
			public:
				using on_new_value_t = utils::void_string_cb;
				
				void set_on_new_value(const on_new_value_t& on_new_value) {
					m_on_new_value = &on_new_value;
				}
				
			public slots:
				void onNewValue(const QString& value);
			
			private:
				const on_new_value_t* m_on_new_value = nullptr;
			};
		}
	}
}
Q_DECLARE_METATYPE(utils::ui::qt::text_input_model)
