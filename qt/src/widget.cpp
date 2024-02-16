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
			int widget::convert_horizontal_alignment(alignment align)
			{
				if (align == alignment::center)
					return Qt::AlignHCenter;
				else if (align == alignment::left)
					return Qt::AlignLeft;
				else if (align == alignment::right)
					return Qt::AlignRight;
				return 0;
			}

			int widget::convert_vertical_alignment(alignment align)
			{
				if (align == alignment::center)
					return Qt::AlignVCenter;
				else if (align == alignment::top)
					return Qt::AlignTop;
				else if (align == alignment::bottom)
					return Qt::AlignBottom;
				return 0;
			}

			int qt::widget::init()
			{
				const QUrl url(u"qrc:abstract_ui_qml/Widget.qml"_qs);
				return qt::widget::init(url);
			}

			int qt::widget::init(const QUrl& component_url, const QVariantMap& initial_properties)
			{
				m_model = create_model();
				auto final_properties = initial_properties;
				final_properties["model"] = QVariant::fromValue(m_model);
				auto r = qt::node::init(component_url, final_properties);
				m_model->connect_to_node(this);
				auto horizontal_alignment = get_horizontal_alignment();
				auto vertical_alignment = get_vertical_alignment();
				auto converted_horizontal_alignment = convert_horizontal_alignment(horizontal_alignment);
				auto converted_vertical_alignment = convert_vertical_alignment(vertical_alignment);
				final_properties["Layout.alignment"] = converted_horizontal_alignment | converted_vertical_alignment;
				return r;
			}

			qt::widget_model* qt::widget::create_model() const
			{
				return new widget_model();
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

			void qt::widget::set_max_width(int value)
			{
				if (m_model)
					m_model->set_max_width(value);
			}
			
			int qt::widget::get_max_width() const
			{
				if (m_model)
					return m_model->max_width();
				return -1;
			}

			void qt::widget::on_horizontal_alignment_set()
			{
				app().do_in_main_thread([self = this]() {
					// TODO: this seems to be not working
					// TODO: Also, make it be friendly with a general dialog centering in ui/widgets/dialog.h
					if (auto object = self->widget_qobject())
					{
						auto current_value = object->property("Layout.alignment").toInt();
						auto new_alignment = self->get_horizontal_alignment();
						auto converted_alignment = convert_horizontal_alignment(new_alignment);
						object->setProperty("Layout.alignment", current_value | converted_alignment);
					}
					return 0;
				});
			}

			void qt::widget::on_vertical_alignment_set()
			{
				app().do_in_main_thread([self = this]() {
					// TODO: this seems to be not working
					// TODO: Also, make it be friendly with a general dialog centering in ui/widgets/dialog.h
					if (auto object = self->widget_qobject())
					{
						auto current_value = object->property("Layout.alignment").toInt();
						auto new_alignment = self->get_vertical_alignment();
						auto converted_alignment = convert_vertical_alignment(new_alignment);
						object->setProperty("Layout.alignment", current_value | converted_alignment);
					}
					return 0;
				});
			}
		}
	}
}