#pragma once

#include <abstract_ui/qt/widget.h>
#include <abstract_ui/widgets/layouts/auto_layout.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class auto_layout : public ui::auto_layout, public virtual qt::widget
			{
			public:
				using base = ui::auto_layout;

			protected:
				int init() override;
			};
		}
	}
}