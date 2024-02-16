#pragma once

#include <memory>
#include <abstract_ui/final/fwd.h>

namespace utils
{
	namespace ui
	{
		class entity;
		class node;
		class app;
		class widget;
		class window;
		class dialog;
		class widget_factory;
		class text;
		class label;
		class button;
		class text_input;
		class menu;
		class menu_manager;
		class menu_controller;
		class view_controller;
		using entity_ptr = std::shared_ptr<entity>;
		using node_ptr = std::shared_ptr<node>;
		using window_ptr = std::shared_ptr<window>;
		using widget_ptr = std::shared_ptr<widget>;
		using dialog_ptr = std::shared_ptr<dialog>;
		using text_ptr = std::shared_ptr<text>;
		using label_ptr = std::shared_ptr<label>;
		using button_ptr = std::shared_ptr<button>;
		using text_input_ptr = std::shared_ptr<text_input>;
		using menu_ptr = std::shared_ptr<menu>;
		using menu_controller_ptr = std::shared_ptr<menu_controller>;
		using view_controller_ptr = std::shared_ptr<view_controller>;
	}
}