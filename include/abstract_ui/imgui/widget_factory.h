#pragma once

#include <memory>
#include <abstract_ui/widget_factory.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class widget_factory : public utils::ui::widget_factory
			{
			public:
				static widget_factory& instance();

			public:
				template <typename Base, typename Final>
				static void register_creator() {
					if (!m_creators)
						m_creators = new creators_t();
					(*m_creators)[typeid(Base).name()] = [](ui::node* parent) {
						auto ptr = std::make_shared<Final>();
						parent->add_node(ptr);
						ptr->post_construct(); // Allow to run code after the constructor worked out
						return ptr;
					};
				}

			protected:
				creators_t& get_creators() const override {
					return *m_creators;
				}
			private:
				static creators_t* m_creators;
			};
		}
	}
}