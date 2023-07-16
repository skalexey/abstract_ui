#include <QUrl>
#include <QString>
#include <QtGlobal>
#include <QQuickWindow>
#include <utils/string_utils.h>
#include <abstract_ui/qt/widgets/dialog.h>
#include <abstract_ui/qt/app.h>
#include <utils/Log.h>
#ifdef LOG_ON
#include <QDebug>
#endif
#include "QtGUI/include/app.h"

LOG_POSTFIX("\n");
LOG_PREFIX("[qt::dialog]: ");

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
				const QUrl url(u"qrc:QtGUI/Dialog.qml"_qs);
				QVariantMap initial_properties;
				initial_properties["title"] = QString(get_title().c_str());
				initial_properties["modal"] = is_modal();
				auto r = qt::node::init(url, initial_properties);
				Q_ASSERT(qobject());
				if (r != 0)
					return r;

				m_content = qobject()->findChild<QObject*>("content");

				QVariant result = qobject()->property("show");
				if (result.canConvert<QJSValue>()) {
					QJSValue jsFunction = result.value<QJSValue>();
					if (jsFunction.isCallable()) {
						QJSValueList args;
						// LOG_DEBUG("Call jsFunction from thread '" << std::this_thread::get_id() << "'");
						QJSValue returnValue = jsFunction.call(args);
					}
				}

				return 0;
			}

			void qt::dialog::on_set_title()
			{
 				if (auto object = qobject())
					object->setProperty("title", QString(get_title().c_str()));
			}

			void qt::dialog::on_set_modal()
			{
				if (auto object = qobject())
					object->setProperty("modal", is_modal());
			}

			QObject* qt::dialog::content_qobject()
			{
				return m_content;
			}

			void qt::dialog::on_before_show()
			{
				qt::window::on_before_show();
			}

			bool qt::dialog::qt_dialog_update(float dt)
			{
				if (!is_open())
					return false;

				// Qt-dialog-specific stuff
				// if (is_auto_resize())
				// 	window_flags |= qtWindowFlags_AlwaysAutoResize;

				// auto label = utils::format_str("%s##", get_title().c_str());
				// bool p_open = true;
				// auto p_open_ptr = is_close_button_enabled() ? &p_open : nullptr;
				return on_qt_dialog_update(dt);
			}

			bool qt::dialog::on_update(float dt)
			{
				if (!qt_dialog_update(dt))
					return false;
				return base::on_update(dt);
			}
		}
	}
}