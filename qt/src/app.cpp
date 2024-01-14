#include <chrono>
#include <QQuickWindow>
#include <QList>
#include <QObject>
#include <QQuickItem>
#include <QQuickStyle>
#include <abstract_ui/qt/app.h>
#include <utils/file_utils.h>
#include <abstract_ui/qt/app_environment.h>
#include <abstract_ui/qt/import_qml_components_plugins.h>
#include <abstract_ui/qt/import_qml_plugins.h>
#include <abstract_ui/qt/register_widgets.h>
#include <utils/Log.h>

LOG_PREFIX("[qt::app]: ");
LOG_POSTFIX("\n");

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			app::app(int argc, char* argv[])
				: base::app(argc, argv)
				, QGuiApplication(argc, argv)
				, m_timer(this)
			{
				connect(&m_timer, &QTimer::timeout, this, &app::update_qt);
				m_timer.start();
			}
			
			int app::init()
			{
				QQuickStyle::setStyle("fusion");
				QList<QObject*> rootObjects = engine().rootObjects();
				for (QObject* obj : rootObjects)
					if (m_main_window = qobject_cast<QQuickWindow*>(obj))
						break;
				if (!m_main_window) {
					LOG_WARNING("Could not find main window");
					return -1;
				}
				qt::node::init(m_main_window);
				return 0;
			}

            QObject* app::content_qobject()
			{
				return m_main_window->contentItem();
			}

			int app::on_run()
			{
				qt::register_widgets();
				
				// Setup Qt
				set_qt_environment();

				engine().addImportPath("qrc:/imports");
				const QUrl url(u"qrc:Main/main.qml"_qs);
				QObject::connect(
					&engine(), &QQmlApplicationEngine::objectCreated, this,
					[url](QObject *obj, const QUrl &objUrl) {
						if (!obj && url == objUrl)
							QCoreApplication::exit(-1);
					},
					Qt::QueuedConnection);

				 engine().addImportPath(QCoreApplication::applicationDirPath() + "/qml");
				 engine().addImportPath(":/");

				engine().load(url);

				if (engine().rootObjects().isEmpty()) {
					return -1;
				}

				auto ret = init();
				if (ret != 0)
					return ret;
				return exec();
			}

			void app::update_qt()
			{
				using namespace std::chrono;
				long long ns = 0;
				static auto start = steady_clock::now();
				auto end = steady_clock::now();
				ns = duration_cast<nanoseconds>(end - start).count();
				if (!app::update((float(ns) * float(1e-9))))
					quit();
				start = end;
			}
			
			bool app::update(float dt)
			{
				bool active = base::update(dt);;
				processEvents(QEventLoop::AllEvents);
				return active;
			}
		}
	}
}
