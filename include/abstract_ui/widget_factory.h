#pragma once

#ifdef LOG_ON
	#include <iostream>
#endif
#include <cassert>
#include <cstdarg>
#include <memory>
#include <typeinfo>
#include <string>
#include <unordered_map>
#include <functional>
#include <abstract_ui/fwd.h>
#include <abstract_ui/node.h>

namespace utils
{
	namespace ui
	{
		class widget_factory
		{
		public:
			template <typename T>
			std::shared_ptr<T> create(node* parent = nullptr) const {
				auto it = m_creators.find(typeid(T).name());
#ifdef LOG_ON
				if (it == m_creators.end())
					std::cout << "Can't find the type '" << typeid(T).name() << "'" << std::endl;
#endif
                assert(it != m_creators.end() && "Type not found");
				if (it != m_creators.end())
					return std::dynamic_pointer_cast<T>(it->second(parent));
				return nullptr;
			}

			template <typename Base, typename Final>
			void register_creator() {
				m_creators[typeid(Base).name()] = [](ui::node* parent) {
					auto ptr = std::make_shared<Final>();
					parent->add_node(ptr);
					ptr->post_construct(); // Allow to run code after the constructor worked out
					return ptr;
				};
			}

		protected:
			using creator_t = std::function<widget_ptr(node*)>;
			using creators_t = std::unordered_map<std::string, creator_t>;

		private:
			creators_t m_creators;
		};
	}
}
