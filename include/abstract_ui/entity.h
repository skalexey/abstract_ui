#pragma once

#include <memory>

namespace utils
{
	namespace ui
	{
		class entity : public std::enable_shared_from_this<entity>
		{
			public:
				template <typename T>
				T& as() {
					return dynamic_cast<T&>(*this);
				}
				template <typename T>
				std::shared_ptr<T> as_ptr() {
					return std::dynamic_pointer_cast<T>(shared_from_this());
				}
				template <typename T>
				bool is() const {
					return dynamic_cast<const T*>(this) != nullptr;
				}
				virtual ~entity() = default;
		};
		using entity_ptr = std::shared_ptr<entity>;
	}
}
