#include <QObject>
#include <QString>
#include <QUrl>
#include <QVariantMap>
#include <abstract_ui/qt/widget_factory.h>
#include <abstract_ui/qt/widgets/clickable_item_model.h>
#include <abstract_ui/qt/widgets/clickable_item.h>
#include <utils/Log.h>

LOG_TITLE("qt::clickable_item");

namespace
{
	REGISTER_WIDGET_DECLARATIVELY(utils::ui::qt::widget_factory, utils::ui::qt::clickable_item);
}

namespace utils
{
	namespace ui
	{
		namespace qt
        {
			int qt::clickable_item::init(const QUrl& url, const QVariantMap& initial_properties)
			{
				QVariantMap final_initial_properties = initial_properties;
				final_initial_properties["text"] = QString(get_text().c_str());
				return qt::widget::init(url, final_initial_properties);
			}

			widget_model* qt::clickable_item::create_model() const
			{
				return new clickable_item_model();
			}

			const clickable_item_model* qt::clickable_item::get_model() const
			{
				return dynamic_cast<const clickable_item_model*>(qt::widget::get_model());
			}

			void qt::clickable_item::on_set_on_click(const on_click_t& on_click)
			{
				model()->set_on_click(on_click);
			}

			const vec2i& qt::clickable_item::text_size()
			{
				// auto s = qt::CalcTextSize(get_text().c_str());
				// return m_calculated_size = { (int)s.x, (int)s.y };
				return m_calculated_size;
			}

			void qt::clickable_item::on_set_text()
			{
				qobject()->setProperty("text", QString(get_text().c_str()));
			}

			void qt::clickable_item::set_enabled(bool enabled)
			{
				qobject()->setProperty("enabled", enabled);
			}

			bool qt::clickable_item::is_enabled() const
			{
				return qobject()->property("enabled").toBool();
			}
		}
	}
}
