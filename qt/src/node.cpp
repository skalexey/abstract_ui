#include <exception>
#include <QObject>
#include <QQuickItem>
#include <QtGlobal>
#include <QQmlContext>
#include <QUrl>
#include <QQmlComponent>
#include <abstract_ui/qt/app.h>
#include <abstract_ui/qt/widget_factory.h>
#include <utils/common_macros.h>
#include <abstract_ui/qt/node.h>
#include <abstract_ui/helpers/user_input.h>
#include <utils/log_stream.h>
#include <utils/Log.h>
SET_LOCAL_LOG_VERBOSE(true);
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
						if (auto parent_ptr = self->parent())
						{
							if (QObject* parent_qbject = parent_ptr->content_qobject())
							{
								LOCAL_VERBOSE("Setting parent to " << parent_qbject);
								finalinitial_properties["parent"] = QVariant::fromValue(parent_qbject);
							}
							else
								LOG_WARNING("Parent content object is null!");
						}

						QQmlComponent component(&self->app().engine(), componentUrl);
						QQmlContext* ctx = self->app().engine().rootContext();
						if (self->m_object = component.createWithInitialProperties(finalinitial_properties, ctx))
							return 0;
						else
						{
							for (const auto& error : component.errors())
							{
								auto&& e = error.toString().toStdString();
								LOG_ERROR("QQmlComponent error: " << e);
								// TODO: make it turn-offable or convert to error codes
								if (auto uinput = dynamic_cast<utils::ui::user_input*>(&self->app()))
									uinput->show_message(e);
							}
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

			void qt::node::on_set_parent(const ui::node* parent)
			{
				// TODO: don't do it if the parent is the same
				app().do_in_main_thread([self = this, parent]() {
					QObject* parent_qobject = nullptr;
					if (parent)
					{
						auto parent_qnode_to_add = dynamic_cast<const qt::node*>(parent);
						auto parent_to_add = parent;
						int level_up = 0;
						while (!parent_qnode_to_add && parent_to_add)
						{
							parent_to_add = parent_to_add->get_parent();
							parent_qnode_to_add = dynamic_cast<const qt::node*>(parent_to_add);
							++level_up;
						}
						if (parent_qnode_to_add)
						{
							parent_qobject = parent_qnode_to_add->content_qobject();
							if (level_up > 0)
							{
								LOG_WARNING(self << "on_set_parent(" << parent << "): The parent is not a qt::node, so this node will be attached to " << level_up << " level above ");
							}
						}
					}
					QObject* qobject = self->content_qobject();
					Q_ASSERT(qobject);
					auto current_parent = qobject->parent();
					if (current_parent != parent_qobject)
						qobject->setParent(parent_qobject);
					return 0;
				});
			}

			QObject* qt::node::parent_qobject() const
			{
				auto content_qobject = this->content_qobject();
				if (!content_qobject)
					return nullptr;
				QObject* parent_qobject = content_qobject->parent();
				if (!parent_qobject)
					content_qobject = m_object->property("parent").value<QObject*>();
				return parent_qobject;
			}
		}
	}
}
