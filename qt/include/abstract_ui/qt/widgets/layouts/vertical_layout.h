#pragma once

#include <abstract_ui/qt/widget.h>
#include <abstract_ui/widgets/layouts/vertical_layout.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class vertical_layout : public ui::vertical_layout, public virtual qt::widget
			{
			public:
				using base = ui::vertical_layout;

			protected:
				int init() override;
			};
		}
	}
}