#pragma once

#include <memory>
#include <string>
#include <abstract_ui/widget.h>
#include <abstract_ui/widgets/text.h>

namespace utils
{
	namespace ui
	{
		// Logically, Label and Text are the same. They only differ in UI implementation.
		class label : public virtual text {};
		using label_ptr = std::shared_ptr<label>;
	}
}