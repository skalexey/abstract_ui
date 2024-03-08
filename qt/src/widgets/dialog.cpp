#include <QUrl>
#include <QString>
#include <QtGlobal>
#include <QQuickWindow>
#include <utils/string_utils.h>
#include <abstract_ui/qt/widgets/dialog.h>
#include <abstract_ui/qt/app.h>
#include <utils/log.h>
#ifdef LOG_ON
#include <QDebug>
#endif

LOG_TITLE("qt::dialog");

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			qt::dialog::dialog()
			{
				set_title("qt Dialog");
			}

			int qt::dialog::init()
			{
				const QUrl url(u"qrc:abstract_ui_qml/Dialog.qml"_qs);
				QVariantMap initial_properties;
				initial_properties["title"] = QString(get_title().c_str());
				initial_properties["modal"] = is_modal();
				auto r = qt::widget::init(url, initial_properties);
				Q_ASSERT(qobject());
				if (r != 0)
					return r;
				m_dialog = qobject()->findChild<QObject*>("dialog");
				// Show the dialog by default.
				show();
				return 0;
			}

			void qt::dialog::on_set_title()
			{
 				if (auto object = widget_qobject())
					object->setProperty("title", QString(get_title().c_str()));
			}

			void qt::dialog::on_set_modal()
			{
				if (auto object = widget_qobject())
					object->setProperty("modal", is_modal());
			}

			void qt::dialog::on_before_show()
			{
				qt::window::on_before_show();
			}

			int qt::dialog::qml_show()
			{
				QVariant result = widget_qobject()->property("show");
				if (result.canConvert<QJSValue>()) {
					QJSValue jsFunction = result.value<QJSValue>();
					if (jsFunction.isCallable()) {
						QJSValueList args;
						// LOG_DEBUG("Call jsFunction from thread '" << std::this_thread::get_id() << "'");
						QJSValue returnValue = jsFunction.call(args);
						return 0;
					}
					else
					{
						LOG_ERROR("jsFunction show() is not callable");
						return -1;
					}
				}
				else
				{
					LOG_ERROR("jsFunction show() is not a QJSValue");
					return -2;
				}
				return 1;
			}

			void qt::dialog::on_show()
			{
				assert(is_initialized() && "Dialog has not been initialized yet?");
				qt::window::on_show();
				// TODO: simplify the code below.
				// Even if it seems not excessive for the cases, it still looks like some not covered cases may exist.
				// Maybe move it to update()?
				auto job = [self = this]() {
					auto parent = self->parent();
					assert(parent);
					if (parent->is_initialized())
						return self->qml_show();
					else
						parent->do_on_post_construct([self]() {
							return self->qml_show();
						});
					return 0;
				};
				if (parent())
					job();
				else
					add_on_set_parent(this, [job]() {
						job();
						return false;
					});
			}

			bool qt::dialog::on_update(float dt)
			{
				if (!base::on_update(dt))
					return false;

				if (!is_open())
					return false;

				// Qt-dialog-specific stuff
				// if (is_auto_resize())
				// 	window_flags |= qtWindowFlags_AlwaysAutoResize;

				// auto label = utils::format_str("%s##", get_title().c_str());
				// bool p_open = true;
				// auto p_open_ptr = is_close_button_enabled() ? &p_open : nullptr;
				return true;
			}
		}
	}
}