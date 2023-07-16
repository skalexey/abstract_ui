#pragma once

#include <QObject>
#include <QString>
#include <utils/common.h>

namespace utils
{
    namespace ui
    {
        namespace qt
        {
            class button_model : public QObject
            {
                Q_OBJECT
            public:
                using on_click_t = utils::void_bool_cb;
                
                explicit button_model(QObject* parent = 0) : QObject(parent) {}
                void set_on_click(const on_click_t& on_click) {
                    m_on_click = on_click;
                }
            public slots:
                void buttonClicked();
                void buttonClicked(QString &in);
            
            private:
                on_click_t m_on_click;
            };
        }
    }
}
Q_DECLARE_METATYPE(utils::ui::qt::button_model)
