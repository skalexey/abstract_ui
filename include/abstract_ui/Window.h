#pragma once

#include <memory>
#include <functional>
#include <string>
#include <utils/vec2.h>
#include <abstract_ui/widget.h>

namespace utils
{
	namespace ui
	{
		class window : public virtual widget
		{
			using base = widget;
			using on_close_cb = base::on_hide_cb;

			public:
				void close() {
					set_visible(false);
					remove_from_parent();
				}

				bool is_open() const {
					return is_visible();
				}

				const std::string& get_title() const override {
					static std::string default_title = "window";
					return default_title;
				};
		};

		using window_ptr = std::shared_ptr<window>;
	}
}