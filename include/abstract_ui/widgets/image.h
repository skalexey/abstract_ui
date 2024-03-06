#pragma once

#include <memory>
#include <string>
#include <abstract_ui/widget.h>

namespace utils
{
	namespace ui
	{
		class image : public virtual widget
		{
		public:
			void set_texture(const std::string& name) {
				m_texture = name;
				on_set_texture();
			}
			const std::string& get_texture() const {
				return m_texture;
			}

		protected:
			virtual void on_set_texture() {}

		private:
			std::string m_texture;
		};
		using image_ptr = std::shared_ptr<image>;
	}
}