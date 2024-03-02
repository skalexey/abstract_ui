#pragma once

#include <memory>
#include <utils/common.h>
#include <utils/ordered_map.h>
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
				for (auto&& entry : m_on_set_title)
					entry.second();
			}

			const void_cb& add_on_set_title(const void* subscriber, const void_cb& cb) {
				return m_on_set_title.add(subscriber, cb);
			}

			const void_cb& set_on_set_title(const void* subscriber, const void_cb& cb) {
				auto it = m_on_set_title.find(subscriber);
				if (it != m_on_set_title.end())
				{
					(*it).second = cb;
					return it->second;
				}
				return m_on_set_title.add(subscriber, cb);
			}

			bool remove_on_set_title(const void* subscriber) {
				return m_on_set_title.erase(subscriber);
			}
			
		protected:
			virtual void on_set_title() {}
			
		private:
			std::string m_title;
			utils::ordered_map<const void*, void_cb> m_on_set_title;
		};
		using menu_list_ptr = std::shared_ptr<menu_list>;
	}
}