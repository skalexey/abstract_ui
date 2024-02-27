#pragma once

#include <memory>
#include <abstract_ui/widgets/menu_list_base.h>

namespace utils
{
	namespace ui
	{
		class menu_list : public virtual menu_list_base
		{
		public:
			using base = menu_list_base;
			const std::string& get_title() const override {
				return m_title;
			}

			void set_title(const std::string& title) {
				m_title = title;
				on_set_title();
			}

		protected:
			virtual void on_set_title() {}
			
		private:
			std::string m_title;
		};
		using menu_list_ptr = std::shared_ptr<menu_list>;
	}
}