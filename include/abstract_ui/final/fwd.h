#pragma once

#include <memory>

namespace utils
{
	namespace ui
	{
		namespace final
		{
			class node;
			class widget;
			class window;
			class dialog;
			using node_ptr = std::shared_ptr<node>;
			using window_ptr = std::shared_ptr<window>;
			using widget_ptr = std::shared_ptr<widget>;
			using dialog_ptr = std::shared_ptr<dialog>;
		}
	}
}