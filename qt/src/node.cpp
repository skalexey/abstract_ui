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
SET_LOCAL_LOG_LEVEL(verbose);
#ifdef LOG_ON
#include <QDebug>
#endif

LOG_TITLE("qt::node");

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

			qt::node::~node()
			{
				destroy();
			}

			void qt::node::on_destroy()
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
					m_object = nullptr;
				}
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

			int qt::node::init(QObject* object, QObject* content_qobject)
			{
				m_object = object;
				if (content_qobject)
					m_content = content_qobject;
				else
				{
					m_content = m_object;
					if (m_object)
						if (auto content_qobject = m_object->findChild<QObject*>("content"))
							m_content = content_qobject;
				}
				return 0;
			}

			int qt::node::init(const QUrl& componentUrl, const QVariantMap& initial_properties)
			{
				return app().do_in_main_thread([self = this, initial_properties, componentUrl] {
					try
					{
						QVariantMap finalinitial_properties = initial_properties;
						auto parent_ptr = self->parent();
						QObject* parent_qobject = nullptr;
						if (parent_ptr)
						{
							assert(parent_ptr->content_qobject());
							if (parent_qobject = parent_ptr->content_qobject())
							{
								LOCAL_VERBOSE("Setting parent to " << parent_qobject);
								// Set the visual parent stored in "parent" property of the created QML Item
								finalinitial_properties["parent"] = QVariant::fromValue(parent_qobject);
							}
							else
								LOG_WARNING("Parent content object is null!");
						}

						QQmlComponent component(&self->app().engine(), componentUrl);
						QQmlContext* ctx = self->app().engine().rootContext();
						if (auto object = component.createWithInitialProperties(finalinitial_properties, ctx))
						{
							if (parent_ptr)
							{
								auto parent_from_property = object->property("parent").value<QObject*>();
								// Not every QML type has a parent property (e.g. QQuickMenuItem), so qobject->property("parent") will return nullptr in such cases.
								// assert(parent_from_property == parent_qobject && "The parent is not the same as the one from the property");
								assert(object->parent() == nullptr);
								object->setParent(parent_qobject); // Set the logical parent, not visual
								QQuickItem* item = qobject_cast<QQuickItem*>(object); // Just for check (in case this is a QQuickItem)
								// item may be nullptr as the node is not always a QQuickItem (e.g. QQuickMenu)
								assert(!item || item && (!item->parentItem() || item->parentItem() && item->parentItem() == qobject_cast<QQuickItem*>(parent_qobject)));
							}
							return self->init(object);
						}
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
							LOG_ERROR("There were errors during creating a node from qml");
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

			int qt::node::post_construct_1()
			{
				// LOG_DEBUG("Calling qt::node::on_post_construct() from thread '" << std::this_thread::get_id() << "'");
				return app().do_in_main_thread([self = this]() {
					RETURN_IF_NE_0(self->base::post_construct_1());
                    return 0;
				});
			}

			void qt::node::on_set_parent(ui::node* parent)
			{
				auto job = [self = this, parent] {
					QObject* parent_qobject = nullptr;
					if (parent)
					{
						auto parent_qnode_to_add = dynamic_cast<const qt::node*>(parent);
						auto parent_to_add = parent;
						int level_up = 0;
						while (!parent_qnode_to_add && parent_to_add)
						{
							parent_to_add = parent_to_add->parent();
							parent_qnode_to_add = dynamic_cast<const qt::node*>(parent_to_add);
							++level_up;
						}
						if (parent_qnode_to_add)
						{
							parent_qobject = parent_qnode_to_add->content_qobject();
							if (level_up > 0)
								LOG_WARNING(self << " on_set_parent(" << parent << "): The parent is not a qt::node, so this node will be attached to " << level_up << " level above ");
						}
					}
					if (!parent_qobject)
						return -1;
					QObject* qobject = self->qobject();
					Q_ASSERT(qobject);
					auto current_parent = qobject->parent();
					if (current_parent != parent_qobject)
					{
						auto parent_from_property = qobject->property("parent").value<QObject*>();
						assert(parent_from_property == current_parent && "Every node should have set its QObject parent to the one from the 'parent' property in qt::node::init()");
						if (parent_from_property != parent_qobject)
						{
							// Set the visual parent stored in "parent" property of the created QML Item.
							// We can't use item->setParentItem(parent_qitem) because the node is not always a QQuickItem (e.g. QQuickMenu)
							qobject->setProperty("parent", QVariant::fromValue(parent_qobject));
							//item->setParentItem(parent_qitem);
							auto new_parent_from_property = qobject->property("parent").value<QObject*>();
							// Some QML types may not have a parent property (e.g. QQuickMenuItem), so qobject->property("parent") will return nullptr
							qobject->setParent(parent_qobject); // Set the logical parent, not visual
						}
					}
					current_parent = qobject->parent();
					assert(current_parent == parent_qobject);
					return 0;
				};
				//assert(is_initialized() && "Please, initialize the object prior to adding");
				if (!is_initialized())
					do_on_post_construct(job);
				else if (app().do_in_main_thread(job) != 0)
					parent->add_on_set_parent(this, [job]() {
						auto result = job();
						if (result != 0)
							LOG_ERROR("on_set_parent() failed with result " << result);
						assert(result == 0);
						return false;
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
