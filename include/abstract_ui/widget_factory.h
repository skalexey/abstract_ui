#pragma once

#include <cassert>
#include <cstdarg>
#include <memory>
#include <typeinfo>
#include <string>
#include <unordered_map>
#include <functional>
#include <abstract_ui/fwd.h>

namespace utils
{
	namespace ui
	{
		class widget_factory
		{
		public:
			template <typename T>
			std::shared_ptr<T> create(node* parent = nullptr) const {
				auto& creators = get_creators();
				auto it = creators.find(typeid(T).name());
                assert(it != creators.end() && (std::string("Not found the type '") + typeid(T).name() + "'").c_str());
				if (it != creators.end())
					return std::dynamic_pointer_cast<T>(it->second(parent));
				return nullptr;
			}

		protected:
			using creator_t = std::function<widget_ptr(node*)>;
			using creators_t = std::unordered_map<std::string, creator_t>;
			virtual creators_t& get_creators() const = 0;
		};
	}
}
