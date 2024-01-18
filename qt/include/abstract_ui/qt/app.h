// 
// Qt Application UI logic controller.

#pragma once

#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QQmlEngine>
#include <QTimer>
#include <abstract_ui/app.h>
#include <abstract_ui/qt/node.h>

class QQuickWindow;

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class app : public virtual qt::node, public virtual utils::ui::app, public QGuiApplication
			{
				using base = ui::app;

			public:
				app(int argc, char* argv[]);
				bool update(float dt) override final;
				QQmlApplicationEngine& engine() {
					return m_application_engine;
				}

				QQmlEngine& qml_engine() {
					return m_qml_engine;
				}

				QObject* content_qobject() const override;

			protected:
				void update_qt();
				int on_run() override final;
				int init() override;
				QTimer m_timer;
				QQmlApplicationEngine m_application_engine;
				QQmlEngine m_qml_engine;
				QQuickWindow* m_main_window = nullptr;
			};
		}
	}
}
