#pragma once

#include <memory>
#include <string>
#include <abstract_ui/qt/fwd.h>
#include <abstract_ui/qt/widgets/text.h>
#include <abstract_ui/widgets/label.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class label : public qt::text, public virtual ui::label
			{
			public:
				using base = ui::label;
				
			protected:
				virtual std::string qml_file_name() const override {
					return "Label.qml";
				}

			private:
				WIDGET_REGISTRATOR(qt::widget_factory, qt::label);
			};
			
			using label_ptr = std::shared_ptr<qt::label>;
		}
	}
}