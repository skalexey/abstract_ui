#pragma once

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
				widget_model(qt::node* node, QObject* parent = nullptr);
                vl::VarPtr grab_update();

            public slots:
                void xChanged();
                void yChanged();
                void widthChanged();
                void heightChanged();

            private:
                qt::node* m_node = nullptr;
                QQmlProperty m_x;
				QQmlProperty m_y;
				QQmlProperty m_width;
				QQmlProperty m_height;
                vl::Object m_update_info;
            };
        }
    }
}
Q_DECLARE_METATYPE(utils::ui::qt::widget_model)
