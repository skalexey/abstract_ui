#pragma once

#include <utils/Log.h>
#include <abstract_ui/qt/widgets/text_input_model.h>

LOG_PREFIX("[qt::text_input_model]: ");
LOG_POSTFIX("\n");

namespace utils
{
	namespace ui
	{
		namespace qt
        {
            void text_input_model::onNewValue(const QString& value) {
				LOG_VERBOSE("onNewValue(" << value.toStdString() << ")");
				if (m_on_new_value)
                    (*m_on_new_value)(value.toStdString());
			}
		}
	}
}
