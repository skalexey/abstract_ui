#include <QUrl>
#include <QString>
#include <QVariantMap>
#include <abstract_ui/qt/widget_factory.h>
#include <abstract_ui/qt/widgets/text.h>
#include <abstract_ui/qt/app.h>

namespace
{
	REGISTER_WIDGET_DECLARATIVELY(utils::ui::qt::widget_factory, utils::ui::qt::text);
}

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			int qt::text::init()
			{
				std::string file_name = "qrc:abstract_ui_qml/" + qml_file_name();
				auto url = QUrl(QString(file_name.c_str()));
				QVariantMap initial_properties;
				auto horizontal_alignment = get_text_horizontal_alignment();
				initial_properties["horizontalAlignment"] = convert_horizontal_alignment(horizontal_alignment);
				auto vertical_alignment = get_text_vertical_alignment();
				initial_properties["verticalAlignment"] = convert_vertical_alignment(vertical_alignment);
				auto r = qt::widget::init(url, initial_properties);
				if (r != 0)
					return r; // For putting BP here while debugging
				return 0;
			}

			void qt::text::set_text(const std::string& text)
			{
				app().do_in_main_thread([self = this, text]() {
					if (auto object = self->qobject())
						object->setProperty("text", QString(text.c_str()));
					return 0;
				});
			}

			const std::string& qt::text::get_text() const
			{
				if (auto object = qobject())
					m_tmp_text = object->property("text").toString().toStdString();
				return m_tmp_text;
			}

			void qt::text::on_set_text_horizontal_alignment()
			{
				app().do_in_main_thread([self = this]() {
					if (auto object = self->qobject())
					{
						auto horizontal_alignment = self->get_text_horizontal_alignment();
						auto converted_alignment = convert_horizontal_alignment(horizontal_alignment);
						object->setProperty("horizontalAlignment", converted_alignment);
					}
					return 0;
				});
			}

			void qt::text::on_set_text_vertical_alignment()
			{
				app().do_in_main_thread([self = this]() {
					if (auto object = self->qobject())
					{
						auto vertical_alignment = self->get_text_vertical_alignment();
						auto converted_alignment = convert_vertical_alignment(vertical_alignment);
						object->setProperty("verticalAlignment", converted_alignment);
					}
					return 0;
				});
			}
		}
	}
}