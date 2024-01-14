#pragma once

#include <abstract_ui/widgets/dialog.h>
#include <abstract_ui/qt/window.h>

class QObject;

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class dialog : public qt::window, public virtual ui::dialog
			{
			public:
				using base = ui::dialog;

				// We don't pass the title as argument because of virtual inheritance
				dialog();
				QObject* content_qobject() const override {
					return m_content;
				}
				QObject* widget_qobject() const override {
					return m_dialog;
				}

			protected:
				void on_before_show() override;
				void on_set_title() override;
				int init() override;
				void on_set_modal() override;

			private:
				bool on_update(float dt) override final;

			private:
				QObject* m_content = nullptr;
				QObject* m_dialog = nullptr;
			};
			using dialog_ptr = std::shared_ptr<dialog>;
		}
	}
}