#pragma once

#include <abstract_ui/qt/widget.h>
#include <abstract_ui/widgets/layouts/horizontal_layout.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class horizontal_layout : public ui::horizontal_layout, public virtual qt::widget
			{
			public:
				using base = ui::horizontal_layout;

			protected:
				int init() override;
			};
		}
	}
}