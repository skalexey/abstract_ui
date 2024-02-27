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
			class clickable_item_model : public widget_model
			{
				Q_OBJECT
				using base = widget_model;

			public:
				using on_click_t = utils::void_bool_cb;
				
				void set_on_click(const on_click_t& on_click) {
					m_on_click = on_click;
				}
				
			public slots:
				void itemClicked();
				void itemClicked(QString &in);
			
			private:
				on_click_t m_on_click;
			};
		}
	}
}
Q_DECLARE_METATYPE(utils::ui::qt::clickable_item_model)
