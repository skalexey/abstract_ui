#pragma once

#include <memory>
#include <abstract_ui/widget_factory.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class widget_factory : public utils::ui::widget_factory
			{
			public:
				static widget_factory& instance() {
					static qt::widget_factory inst;
					return inst;
				}
			};
		}
	}
}