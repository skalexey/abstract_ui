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
			menu_manager(ui::app& app) : m_app(&app), m_data(m_all_data[m_app]) {}
			void open_menu(const std::string& name, const vl::Object& options = vl::NullObject());
			void back();
			void close_current_menu() {
				back();
			}
			void close_all_menus() {
				while (!m_data.menu_stack.empty())
					back();
			}
			int get_menu_stack_size() const {
				return m_data.menu_stack.size();
			}
			template <typename T>
			void register_menu(const std::string& name) {
				data().registry.register_creator(name, [app = m_app](const vl::Object& options) {
					auto ptr = utils::ui::widget_factory::create_abstract<T>(app, options);
					return ptr;
				});
			}

		protected:
			void load_current_menu();
			bool unload_current_menu();

		private:
			class menu_registry
			{
				using creator_t = std::function<ui::menu_controller_ptr(const vl::Object&)>;
				std::unordered_map<std::string, creator_t> m_menus;
				
			public:
				void register_creator(const std::string& name, const creator_t& creator) {
					m_menus[name] = creator;
				}
				ui::menu_controller_ptr create_menu(const std::string& name, const vl::Object& options = nullptr);
			};
			struct menu_manager_data
			{
				menu_registry registry;
				struct 
				{
					std::string name;
					ui::menu_controller_ptr menu;
				} current_menu;
				struct menu_stack_entry
				{
					std::string name;
					vl::Object options;
				};
				std::vector<menu_stack_entry> menu_stack;
			};
			menu_manager_data& data() {
				return m_data;
			}

		private:
			ui::app* m_app = nullptr;
			static std::unordered_map<ui::app*, menu_manager_data> m_all_data;
			menu_manager_data& m_data;
		};
	}
}