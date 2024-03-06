#pragma once

#include <memory>
#include <abstract_ui/qt/fwd.h>
#include <abstract_ui/widgets/image.h>
#include <abstract_ui/qt/widget.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class image : public virtual qt::widget, public virtual ui::image
			{
			public:
				using base = ui::image;

				int init() override;

			protected:
				void on_set_texture() override;
			};
			using image_ptr = std::shared_ptr<image>;
		}
	}
}