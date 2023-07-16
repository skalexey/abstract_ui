#include <exception>
#include <QtGlobal>
#include <QQmlContext>
#include <QUrl>
#include <QQmlComponent>
#include <abstract_ui/qt/app.h>
#include <abstract_ui/qt/widget_factory.h>
#include <utils/common_macros.h>
#include <abstract_ui/qt/node.h>
#include <utils/Log.h>
#ifdef LOG_ON
#include <QDebug>
#endif

LOG_POSTFIX("\n");
LOG_PREFIX("[qt::node]: ");

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			qt::node::node()
			{
				set_factory(qt::widget_factory::instance());
			}

			// All the dynamic casts require a fully defined type.
			qt::app& qt::node::app()
			{
				return dynamic_cast<qt::app&>(base::app());
			}

			const qt::app& qt::node::get_app() const
			{
				return dynamic_cast<const qt::app&>(base::get_app());
			}

			int qt::node::init(const QUrl& componentUrl, const QVariantMap& initial_properties)
			{
				return app().do_in_main_thread([self = this, initial_properties, componentUrl] {
					try
					{
						QVariantMap finalinitial_properties = initial_properties;
						auto parent_ptr = self->parent();
						Q_ASSERT(parent_ptr);
						if (!parent_ptr)
							return -2;
						QObject* parentObject = parent_ptr->content_qobject();
						if (parentObject)
							finalinitial_properties["parent"] = QVariant::fromValue(parentObject);
						else
							LOG_WARNING("Parent object is null!");

						QQmlComponent component(&self->app().engine(), componentUrl);
						QQmlContext* ctx = self->app().engine().rootContext();
						if (self->m_object = component.createWithInitialProperties(finalinitial_properties, ctx))
							return 0;
						else
						{
							for (const auto& error : component.errors())
								LOG_ERROR("QQmlComponent error: " << error.toString().toStdString());
							LOG_ERROR("There were errors during creating a dialog from qml");
							return -1;
						}
					}
					catch (std::exception& ex)
					{
						LOG_ERROR("Loading qml exception: " << ex.what());
						return -10;
					}
					catch (...)
					{
						LOG_ERROR("Loading qml exception!");
						return -11;
					}
					return 0;
				});
			}

			int qt::node::on_post_construct()
			{
				// LOG_DEBUG("Calling qt::node::on_post_construct() from thread '" << std::this_thread::get_id() << "'");
				return app().do_in_main_thread([self = this]() {
					RETURN_IF_NE_0(self->base::on_post_construct());
					RETURN_IF_NE_0(self->on_qt_node_post_construct());
                    return 0;
				});
			}
		}
	}
}
