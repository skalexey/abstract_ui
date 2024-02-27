#pragma once

#include <memory>
#include <abstract_ui/widget.h>

namespace utils
{
	namespace ui
	{
		class menu_list_base : public virtual widget
		{
		public:
			using base = widget;
			using action_t = std::function<void()>;
			virtual void add_item(const std::string& title, const action_t& action);
			virtual void add_widget(const widget_ptr& widget) {
				add_node(widget);
			}
		};
		using menu_list_base_ptr = std::shared_ptr<menu_list_base>;
	}
}