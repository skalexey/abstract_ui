#pragma once

#include <memory>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class node;
			class app;
			class widget;
			class widget_model;
			class window;
			class dialog;
			class widget_factory;
			class text;
			class label;
			class button;
			class clickable_item_model;
			class text_input;
			class text_input_model;
			using window_ptr = std::shared_ptr<window>;
			using widget_ptr = std::shared_ptr<widget>;
			using dialog_ptr = std::shared_ptr<dialog>;
			using text_ptr = std::shared_ptr<text>;
			using label_ptr = std::shared_ptr<label>;
			using button_ptr = std::shared_ptr<button>;
			using text_input_ptr = std::shared_ptr<text_input>;
		}
	}
}