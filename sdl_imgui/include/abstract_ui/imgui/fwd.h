#pragma once

#include <memory>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class entity;
			class node;
			class sdl_node;
			class app;
			class sdl_app;
			class widget;
			class window;
			class dialog;
			class widget_factory;
			class text;
			class label;
			class button;
			class text_input;
			using entity_ptr = std::shared_ptr<entity>;
			using node_ptr = std::shared_ptr<node>;
			using sdl_node_ptr = std::shared_ptr<sdl_node>;
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