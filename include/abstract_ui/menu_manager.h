#pragma once

#include <cassert>
#include <functional>
#include <unordered_map>
#include <string>
#include <VL.h>
#include <abstract_ui/fwd.h>

namespace utils
{
	namespace ui
	{
		class menu_manager
		{
		public:
			menu_manager(ui::app& app) : m_app(&app) {}
			void open_menu(const std::string& name, const vl::Object& options = vl::NullObject());
			void close_current_menu();
			void close_all_menus();
			void back();
			template <typename T>
			void register_menu(const std::string& name) {
				data().registry.register_creator(name, [app = m_app](const vl::Object& options) {
					auto ptr = utils::ui::widget_factory::create_abstract<T>(app, options);
					return ptr;
				});
			}

		private:
			class menu_registry
			{
				using creator_t = std::function<ui::menu_controller_ptr(const vl::Object&)>;
				std::unordered_map<std::string, creator_t> m_menus;
				
			public:
				void register_creator(const std::string& name, const creator_t& creator) {
					m_menus[name] = creator;
				}
				ui::menu_controller_ptr create_menu(const std::string& name, const vl::Object& options = vl::NullObject());
			};
			struct menu_manager_data
			{
				menu_registry registry;
				struct 
				{
					std::string name;
					ui::menu_controller_ptr menu;
				} current_menu;
				std::vector<std::string> menu_stack;
			};
			menu_manager_data& data() {
				return m_data[m_app];
			}

		private:
			ui::app* m_app = nullptr;
			static std::unordered_map<ui::app*, menu_manager_data> m_data;
		};
	}
}