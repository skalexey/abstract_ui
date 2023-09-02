#include <QUrl>
#include <QString>
#include <QVariantMap>
#include <abstract_ui/qt/widget_factory.h>
#include <abstract_ui/qt/widgets/text.h>
#include <abstract_ui/qt/app.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			REGISTER_WIDGET_DECLARATIVELY(text);

			int qt::text::init()
			{
				std::string file_name = "qrc:QtGUI/" + qml_file_name();
				auto url = QUrl(QString(file_name.c_str()));
				QVariantMap initial_properties;
				auto r = qt::node::init(url, initial_properties);
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
				if (auto object = get_qobject())
					m_tmp_text = object->property("text").toString().toStdString();
				return m_tmp_text;
			}
		}
	}
}