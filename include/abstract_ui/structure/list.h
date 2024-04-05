#pragma once

#include <vector>
#include <list>
#include <cstddef>
#include <abstract_ui/fwd.h>

#define UI_STRUCTURE_LIST(T, Container_t) utils::ui::structure::list_base<T, Container_t<T>>

namespace utils
{
	namespace ui
	{
		namespace structure
		{
			template <typename T, typename Container_t>
			class list_base
			{
			public:
				list_base(utils::ui::node* owner) : m_owner(owner) {}
				// Simple add() mehod
				T& add() {
					m_elements.emplace_back(m_owner);
					return m_elements.back();
				}
				// add(<args>) method that forwards arguments to T's constructor
				template <typename... Args>
				T& add(Args&&... args) {
					m_elements.emplace_back(m_owner, std::forward<Args>(args)...);
					return m_elements.back();
				}
				T& at(std::size_t index) {
					return m_elements[index];
				}
				std::size_t size() const {
					return m_elements.size();
				}

			protected:
				Container_t m_elements;

			private:
				utils::ui::node* m_owner;
			};

			template <typename T>
			using vector = UI_STRUCTURE_LIST(T, std::vector);

			template <typename T>
			using list = UI_STRUCTURE_LIST(T, std::list);
		}
	}
}
