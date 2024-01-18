#include <QVariant>
#include <QObject>
#include <QJSValue>
#include <VL.h>
#include <abstract_ui/widget.h>
#include <abstract_ui/qt/widget.h>
#include <abstract_ui/qt/app.h>
#include <abstract_ui/qt/widget_model.h>
#include <utils/Log.h>
LOG_POSTFIX("\n");
LOG_TITLE("qt::widget");
namespace utils
{
	namespace ui
	{
		namespace qt
		{
			widget::~widget()
			{
				if (auto object = qobject())
				{
					QVariant result = object->property("onDestroy");
					if (result.canConvert<QJSValue>()) {
						QJSValue jsFunction = result.value<QJSValue>();
						if (jsFunction.isCallable()) {
							QJSValueList args;
							QJSValue returnValue = jsFunction.call(args);
						}
					}
					object->setParent(nullptr);
					object->deleteLater();
				}
			}

			void qt::widget::on_qobject_created()
			{
				m_model = new widget_model(this);
			}
			
			vec2i qt::widget::get_screen_size() const
			{
				// Initial size is specified in Constants.qml
				auto main_window = get_app().content_qobject();
				return vec2i(main_window->property("width").toInt(), main_window->property("height").toInt());
			}

			void qt::widget::set_position(const vec2i& pos)
			{
				if (auto object = qobject())
				{
					object->setProperty("x", pos.x);
					object->setProperty("y", pos.y);
					// This code should trigger on_position_changed in the end of the frame through grabbing the model update
				}
				on_set_position();
			}

			void qt::widget::set_size(const vec2i& size)
			{
				auto&& size_policy = get_size_policy();
				if (size_policy.horizontal == size_policy::type::automatic && size_policy.vertical == size_policy::type::automatic)
						return;
				if (auto object = widget_qobject())
				{
					object->setProperty("width", size.x);
					object->setProperty("height", size.y);
					// This code should trigger on_size_changed in the end of the frame through grabbing the model update
				}
				on_set_size();
			}

			const vec2i& qt::widget::get_size() const
			{
				if (auto object = widget_qobject())
				{
					m_size.x = object->property("width").toInt();
					m_size.y = object->property("height").toInt();
				}
				return m_size;
			}

			const vec2i& qt::widget::get_position() const
			{
				if (auto object = qobject())
				{
					m_position.x = object->property("x").toInt();
					m_position.y = object->property("y").toInt();
				}
				return m_position;
			}

			void qt::widget::collect_property_updates()
			{
				if (!m_model)
					return;
				auto update_ptr = m_model->grab_update();
				if (!update_ptr)
					return;
				auto update = update_ptr->AsObject();
				// Position
				bool has_x = update.Has("x");
				bool has_y = update.Has("y");
				if (has_x)
					m_position.x = update.Get("x").AsNumber().Val();
				if (has_y)
					m_position.y = update.Get("y").AsNumber().Val();
				if (has_x || has_y)
				{
					on_position_changed();
					LOG_VERBOSE("Position changed to (" << m_position.x << ", " << m_position.y << ")");
				}
				// Size
				bool has_width = update.Has("width");
				bool has_height = update.Has("height");
				if (has_width)
					m_size.x = update.Get("width").AsNumber().Val();
				if (has_height)	
					m_size.y = update.Get("height").AsNumber().Val();
				if (has_width || has_height)
					on_size_changed();
			}
		}
	}
}