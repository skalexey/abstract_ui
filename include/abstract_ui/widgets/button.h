#pragma once

#include <memory>
#include <abstract_ui/widgets/clickable_item.h>

namespace utils
{
	namespace ui
	{
		class button : public virtual clickable_item
		{
		public:
			using base = clickable_item;
		};
		using button_ptr = std::shared_ptr<button>;
	}
}