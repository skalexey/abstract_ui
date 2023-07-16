#pragma once

#include <memory>
#include <string>
#include <abstract_ui/qt/fwd.h>
#include <abstract_ui/widgets/text.h>
#include <abstract_ui/qt/widget.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class text : public qt::widget, public virtual ui::text
			{
			public:
				using base = ui::text;

				void set_text(const std::string& text) override;
				const std::string& get_text() const override;

			protected:
				virtual std::string qml_file_name() const {
					return "Text.qml";
				}
				int init() override;
				WIDGET_REGISTRATOR(qt::widget_factory, text);

			private:
				mutable std::string m_tmp_text; // Needed as get_text() returns const std::string&,
				// but Qt can only return std::string created from QString which in turn is created from QVariant.
			};
			using text_ptr = std::shared_ptr<text>;
		}
	}
}