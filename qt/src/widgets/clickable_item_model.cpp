#pragma once

#include <utils/Log.h>
#include <abstract_ui/qt/widgets/clickable_item_model.h>

LOG_TITLE("qt::clickable_item_model");

namespace utils
{
	namespace ui
	{
		namespace qt
        {
            void clickable_item_model::itemClicked() {
				LOG("itemClicked");
				if (m_on_click)
					m_on_click(true);
			}
			void clickable_item_model::itemClicked(QString &in) {
				LOG("itemClicked(" << in.toStdString() << "");
				if (m_on_click)
					m_on_click(true);
			}
		}
	}
}
