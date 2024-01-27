#pragma once

#include <memory>
#include <string>
#include <abstract_ui/widget.h>

namespace utils
{
	namespace ui
	{
		class text : public virtual widget
		{
		public:
			virtual void set_text(const std::string& text) = 0;
			virtual const std::string& get_text() const = 0;
			void set_text_horizontal_alignment(alignment align) {
				m_text_horizontal_alignment = align;
				on_set_text_horizontal_alignment();
			}
			void set_text_vertical_alignment(alignment align) {
				m_text_vertical_alignment = align;
				on_set_text_vertical_alignment();
			}
			alignment get_text_horizontal_alignment() const {
				return m_text_horizontal_alignment;
			}
			alignment get_text_vertical_alignment() const {
				return m_text_vertical_alignment;
			}

		protected:
			virtual void on_set_text() {}
			virtual void on_set_text_horizontal_alignment() {}
			virtual void on_set_text_vertical_alignment() {}

		private:
			alignment m_text_horizontal_alignment = alignment::none;
			alignment m_text_vertical_alignment = alignment::none;
		};
		using text_ptr = std::shared_ptr<text>;
	}
}