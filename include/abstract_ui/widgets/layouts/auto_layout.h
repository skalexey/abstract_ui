#pragma once

#include <abstract_ui/widgets/layout.h>

namespace utils
{
	namespace ui
	{
		class auto_layout : public virtual layout
		{
		public:
			using base = layout;
			auto_layout();
		};
	}
}