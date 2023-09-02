#include <cmath>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQuickItem>
#include <QQmlEngine>
#include <QVariant>
#include <QObject>
#include <QString>
#include <QUrl>
#include <abstract_ui/qt/app.h>
#include <abstract_ui/qt/widgets/button.h>
#include <abstract_ui/qt/widget_factory.h>
#include <abstract_ui/qt/widgets/button_model.h>

#include <utils/Log.h>

LOG_PREFIX("[qt::button] ");
LOG_POSTFIX("\n");

namespace utils
{
	namespace ui
	{
		namespace qt
        {
			REGISTER_WIDGET_DECLARATIVELY(qt::button);

			int qt::button::init()
			{
				const QUrl url(u"qrc:QtGUI/Button.qml"_qs);
				QVariantMap initial_properties;
				initial_properties["text"] = QString(get_text().c_str());
				m_model = new button_model();
				initial_properties["model"] = QVariant::fromValue(m_model);
				auto r = qt::node::init(url, initial_properties);
				if (r != 0)
					return r;
				m_model->setParent(qobject());
				return 0;
			}

			void qt::button::on_set_on_click(const on_click_t& on_click)
			{
				m_model->set_on_click(on_click);
			}

			const vec2i& qt::button::text_size()
			{
				// auto s = qt::CalcTextSize(get_text().c_str());
				// return m_calculated_size = { (int)s.x, (int)s.y };
				return m_calculated_size;
			}

			void qt::button::on_set_text(const std::string& text)
			{
				qobject()->setProperty("text", QString(text.c_str()));
			}
		}
	}
}
