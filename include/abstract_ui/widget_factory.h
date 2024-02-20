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
#include <VL.h>
#include <abstract_ui/fwd.h>
#include <abstract_ui/app.h>
#include <abstract_ui/node.h>

namespace
{
	void set_factory(utils::ui::node* node, const utils::ui::app& app);
	#define BEGIN_DO_IN_MAIN_THREAD() \
		auto create_f = [&]() {

	#define END_DO_IN_MAIN_THREAD(parent) \
		}; \
		std::shared_ptr<T> ptr; \
		if (parent) \
			parent->app().do_in_main_thread([&ptr, create_f]() { \
				ptr = create_f(); \
				return 0; \
			}); \
		else \
			ptr = create_f(); \
		return ptr;

	#define DO_IN_MAIN_THREAD(parent, expr) \
		auto create_f = [&]() { \
			expr; \
		}; \
		std::shared_ptr<T> ptr; \
		if (parent) \
			parent->app().do_in_main_thread([&ptr, create_f]() { \
				ptr = create_f(); \
				return 0; \
			}); \
		else \
			ptr = create_f(); \
		return ptr;
}

namespace utils
{
	namespace ui
	{
		class widget_factory
		{
		public:
			template <typename T>
			static std::shared_ptr<T> create_node(node* parent = nullptr, const vl::Object& options = nullptr, app* app = nullptr, bool deferred = false) {
				// Make sure no update() is called between add_node and post_construct
				BEGIN_DO_IN_MAIN_THREAD()
					auto ptr = std::make_shared<T>();
					if (options)
						ptr->set_options(options);
					if (parent)
						parent->add_node(ptr);
					if (app)
						ptr->m_app = app;
					if (!deferred)
						ptr->post_construct(); // Allow to run code after the constructor worked out
					return ptr;
				END_DO_IN_MAIN_THREAD(parent)
			}

			template <typename T>
			static std::shared_ptr<T> create_abstract(node* parent = nullptr, const vl::Object& options = nullptr, app* app = nullptr, bool deferred = false) {
				BEGIN_DO_IN_MAIN_THREAD()
					auto ptr = create_node<T>(parent, options, app, true);
					ptr->set_factory(ptr->get_app().get_factory());
					if (!deferred)
						ptr->post_construct();
					return ptr;
				END_DO_IN_MAIN_THREAD(parent)
			}

			template <typename T>
			std::shared_ptr<T> create_final(node& parent, const vl::Object& options = nullptr, app* app = nullptr) const {
				BEGIN_DO_IN_MAIN_THREAD()
					auto ptr = std::make_shared<T>();
					ptr->set_options(options);
					parent.add_node(ptr);
					auto impl = create<typename T::impl_t>(ptr.get(), options, app, true);
					ptr->set_impl(impl);
					impl->post_construct();
					ptr->post_construct(); // Allow to run code after the constructor worked out
					return ptr;
				END_DO_IN_MAIN_THREAD((&parent))
			}
			
			template <typename T>
			std::shared_ptr<T> create(node* parent = nullptr, const vl::Object& options = nullptr, app* app = nullptr, bool deferred = false) const {
				auto it = m_creators.find(typeid(T).name());
#ifdef LOG_ON
				if (it == m_creators.end())
					std::cout << "Can't find the type '" << typeid(T).name() << "'" << std::endl;
#endif
                assert(it != m_creators.end() && "Type not found");
				if (it != m_creators.end())
					return std::dynamic_pointer_cast<T>(it->second(parent, options, app, deferred));
				return nullptr;
			}

			template <typename Base, typename Final>
			void register_creator() {
				m_creators[typeid(Base).name()] = [](ui::node* parent, const vl::Object& options, ui::app* app, bool deferred) {
					return create_node<Final>(parent, options, app, deferred);
				};
			}

		protected:
			using creator_t = std::function<widget_ptr(node*, const vl::Object&, app*, bool)>;
			using creators_t = std::unordered_map<std::string, creator_t>;

		private:
			creators_t m_creators;
		};
	}
}
