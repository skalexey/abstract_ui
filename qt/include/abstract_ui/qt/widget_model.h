#pragma once

#include <climits>
#include <VL.h>
#include <QObject>
#include <QQmlProperty>
#include <abstract_ui/qt/node.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class widget_model : public QObject
			{
				Q_OBJECT

			public:
				vl::VarPtr grab_update();
				void connect_to_node(qt::node* node);

			public slots:
			// Parent widget watching slots
				void xChanged();
				void yChanged();
				void widthChanged();
				void heightChanged();

			signals:
				void maximumWidthChanged();

			public:
			// Properties
				Q_PROPERTY(int maximumWidth READ max_width WRITE set_max_width NOTIFY maximumWidthChanged)
				int max_width() const {
					return m_max_width;
				}
				void set_max_width(int value) {
					m_max_width = value;
					emit maximumWidthChanged();
				}

			private:
				qt::node* m_node = nullptr;
				QQmlProperty m_x;
				QQmlProperty m_y;
				QQmlProperty m_width;
				QQmlProperty m_height;
				vl::Object m_update_info;
				int m_max_width = INT_MAX;
			};
		}
	}
}
Q_DECLARE_METATYPE(utils::ui::qt::widget_model)
