#pragma once

#include <memory>
#include <abstract_ui/final/fwd.h>

namespace utils
{
	class entity;
	namespace ui
	{
		class node;
		class app;
		class widget;
		class layout;
		class vertical_layout;
		class horizontal_layout;
		class auto_layout;
		class window;
		class dialog;
		class widget_factory;
		class text;
		class label;
		class clickable_item;
		class button;
		class text_input;
		class menu_item;
		class menu_list_base;
		class menu_list;
		class menu_bar;
		class menu;
		class image;
		class menu_manager;
		class menu_controller;
		class view_controller;
		using entity_ptr = std::shared_ptr<entity>;
		using node_ptr = std::shared_ptr<node>;
		using window_ptr = std::shared_ptr<window>;
		using widget_ptr = std::shared_ptr<widget>;
		using layout_ptr = std::shared_ptr<layout>;
		using vertical_layout_ptr = std::shared_ptr<vertical_layout>;
		using horizontal_layout_ptr = std::shared_ptr<horizontal_layout>;
		using auto_layout_ptr = std::shared_ptr<auto_layout>;
		using dialog_ptr = std::shared_ptr<dialog>;
		using text_ptr = std::shared_ptr<text>;
		using label_ptr = std::shared_ptr<label>;
		using clickable_item_ptr = std::shared_ptr<clickable_item>;
		using button_ptr = std::shared_ptr<button>;
		using text_input_ptr = std::shared_ptr<text_input>;
		using menu_item_ptr = std::shared_ptr<menu_item>;
		using menu_list_base_ptr = std::shared_ptr<menu_list_base>;
		using menu_list_ptr = std::shared_ptr<menu_list>;
		using menu_bar_ptr = std::shared_ptr<menu_bar>;
		using menu_ptr = std::shared_ptr<menu>;
		using image_ptr = std::shared_ptr<image>;
		using menu_controller_ptr = std::shared_ptr<menu_controller>;
		using view_controller_ptr = std::shared_ptr<view_controller>;
	}
}