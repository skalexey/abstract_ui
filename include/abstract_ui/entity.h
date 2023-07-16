#pragma once

#include <memory>

namespace utils
{
	namespace ui
	{
		class entity
		{
			public:
				virtual ~entity() = default;
		};
		using entity_ptr = std::shared_ptr<entity>;
	}
}
