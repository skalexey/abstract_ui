#pragma once

#include <abstract_ui/widgets/dialog.h>

namespace utils
{
	namespace ui
	{
		class modal_dialog : public virtual dialog
		{
		public:
			modal_dialog() {
				set_modal(true);
			}
		};
	}
}