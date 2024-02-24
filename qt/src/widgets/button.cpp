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

LOG_TITLE("qt::button");

namespace
{
	REGISTER_WIDGET_DECLARATIVELY(utils::ui::qt::widget_factory, utils::ui::qt::button);
}

namespace utils
{
	namespace ui
	{
		namespace qt
        {
			int qt::button::init()
			{
				const QUrl url(u"qrc:abstract_ui_qml/Button.qml"_qs);
				QVariantMap initial_properties;
				initial_properties["text"] = QString(get_text().c_str());
				return qt::widget::init(url, initial_properties);
			}

			widget_model* qt::button::create_model() const
			{
				return new button_model();
			}

			const button_model* qt::button::get_model() const
			{
				return dynamic_cast<const button_model*>(qt::widget::get_model());
			}

			void qt::button::on_set_on_click(const on_click_t& on_click)
			{
				model()->set_on_click(on_click);
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

			void qt::button::set_enabled(bool enabled)
			{
				qobject()->setProperty("enabled", enabled);
			}
		}
	}
}
