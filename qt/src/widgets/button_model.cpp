#pragma once

#include <utils/Log.h>
#include <abstract_ui/qt/widgets/button_model.h>

LOG_PREFIX("[qt::button_model]: ");
LOG_POSTFIX("\n");

namespace utils
{
	namespace ui
	{
		namespace qt
        {
            void button_model::buttonClicked() {
				LOG("buttonClicked");
				if (m_on_click)
					m_on_click(true);
			}
			void button_model::buttonClicked(QString &in) {
				LOG("buttonClicked(" << in.toStdString() << "");
				if (m_on_click)
					m_on_click(true);
			}
		}
	}
}
