#pragma once

#include <memory>
#include <abstract_ui/qt/node.h>
#include <abstract_ui/widget.h>

class QObject;

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class widget : public virtual qt::node, public virtual ui::widget
			{
			public:
				~widget() override;
				
				vec2i get_screen_size() const override;

				widget* as_widget() override {
					return this;
				}
			};
			using widget_ptr = std::shared_ptr<qt::widget>;
		}
	}
}