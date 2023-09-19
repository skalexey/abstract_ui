#pragma once

#include <memory>
#include <abstract_ui/widget_factory.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class widget_factory : public utils::ui::widget_factory
			{
			public:
				static widget_factory& instance() {
					static imgui::widget_factory inst;
					return inst;
				}

			private:
				static creators_t* m_creators;
			};
		}
	}
}