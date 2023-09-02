#include <QtGlobal>
#include <QQuickItem>
#include <abstract_ui/widgets/text_input.h>
#include <utils/io_utils.h>
#include <abstract_ui/qt/app.h>
#include <abstract_ui/qt/widget_factory.h>
#include <abstract_ui/qt/widgets/text_input.h>

namespace
{
	std::string empty_string;
}

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			REGISTER_WIDGET_DECLARATIVELY(text_input);

			int qt::text_input::init()
			{
				const QUrl url(u"qrc:QtGUI/TextInput.qml"_qs);
				QVariantMap initial_properties;
				initial_properties["text"] = QString(get_default_value().c_str());
				m_model = new text_input_model();
				initial_properties["model"] = QVariant::fromValue(m_model);
				auto r = qt::node::init(url, initial_properties);
				if (r != 0)
					return r;
				m_model->setParent(qobject());
				m_text_field = qobject()->findChild<QQuickItem*>("input");
				m_label = qobject()->findChild<QQuickItem*>("label");
				return 0;
			}

			void qt::text_input::on_set_on_new_value()
			{
				m_model->set_on_new_value(get_on_new_value());
			}

			const std::string& qt::text_input::get_value() const
			{
				if (auto object = get_qobject())
				{
					Q_ASSERT(get_text_field());
					return m_tmp_value = get_text_field()->property("text").toString().toStdString();
				}
				return empty_string;
			}

			void qt::text_input::set_value(const std::string& value)
			{
				app().do_in_main_thread([self = this, value]() {
					if (auto object = self->text_field())
						object->setProperty("text", QString(value.c_str()));
					return 0;
				});
			}

			const QQuickItem* qt::text_input::get_text_field() const
			{
				return m_text_field;
			}

			QQuickItem* qt::text_input::text_field()
			{
				return m_text_field;
			}

			const QQuickItem* qt::text_input::get_label() const
			{
				return m_label;
			}

			QQuickItem* qt::text_input::label()
			{
				return m_label;
			}
		}
	}
}