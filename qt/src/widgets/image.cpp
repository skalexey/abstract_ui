#include <filesystem>
#include <QVariant>
#include <QObject>
#include <utils/log.h>
#include <abstract_ui/qt/app.h>
#include <abstract_ui/qt/widget_factory.h>
#include <abstract_ui/qt/widgets/image.h>

LOG_TITLE("qt::image");

namespace
{
	REGISTER_WIDGET_DECLARATIVELY(utils::ui::qt::widget_factory, utils::ui::qt::image);

	std::string texture_path(const std::string& texture)
	{
		std::filesystem::path path(texture);
		if (path.is_absolute())
			return texture;
		return "qrc:content/assets/textures/" + texture;
	}
}

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			void qt::image::on_set_texture()
			{
				app().do_in_main_thread([self = this]() {
					if (auto object = self->qobject())
					{
						const QUrl url(QString::fromStdString(texture_path(self->get_texture())));
						object->setProperty("source", url);
					}
					return 0;
				});
			}
			
			int qt::image::init()
			{
				const QUrl url(u"qrc:abstract_ui_qml/Image.qml"_qs);
				QVariantMap initial_properties;
				auto& texture = get_texture();
				if (!texture.empty())
				{
					const QUrl texture_url(QString::fromStdString(texture_path(get_texture())));
					initial_properties["source"] = texture_url;
				}
				return qt::widget::init(url, initial_properties);
			}
		}
	}
}