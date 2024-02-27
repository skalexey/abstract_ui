#pragma once

#include <memory>
#include <abstract_ui/qt/fwd.h>
#include <abstract_ui/widgets/button.h>
#include <abstract_ui/qt/widgets/clickable_item.h>

namespace utils
{
	namespace ui
	{
		namespace qt
        {
			class button : public qt::clickable_item, public virtual ui::button
			{
			public:
				using base = ui::button;

			protected:
				int init() override;
			};
			using button_ptr = std::shared_ptr<button>;
		}
	}
}
