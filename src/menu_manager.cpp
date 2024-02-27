#pragma once

#include <string>
#include <abstract_ui/menu_manager.h>
#include <abstract_ui/widgets/menu.h>
#include <abstract_ui/menu_controller.h>
#include <utils/Log.h>
LOG_TITLE("menu_manager");

namespace utils
{
	namespace ui
	{
		std::unordered_map<ui::app*, menu_manager::menu_manager_data> menu_manager::m_all_data;

		ui::menu_controller_ptr menu_manager::menu_registry::create_menu(const std::string& name, const vl::Object& options)
		{
			auto it = m_menus.find(name);
			assert(it != m_menus.end());
			try
			{
				return it->second(options);
			}
			catch (std::exception& ex)
			{
				LOG("Creating controller exception: " << ex.what());
			}
			catch (...)
			{
				LOG("Creating controller exception!");
			}
			return nullptr;
		}

		void menu_manager::open_menu(const std::string& name, const vl::Object& options)
		{
			if (m_data.current_menu.name == name)
			{
				current_menu().process_event("open", options);
				return;
			}
			if (m_data.current_menu.menu)
				unload_current_menu();
			m_data.menu_stack.push_back({name, options});
			load_current_menu();
		}

		void menu_manager::back()
		{
			if (unload_current_menu())
				m_data.menu_stack.pop_back();
			if (!m_data.menu_stack.empty())
				load_current_menu();
		}

		void menu_manager::load_current_menu()
		{
			auto& current_menu_entry = m_data.menu_stack.back();
			auto& name = current_menu_entry.name;
			auto& options = current_menu_entry.options;
			assert(name != m_data.current_menu.name);
			auto menu = m_data.registry.create_menu(name, options);
			assert(menu);
			m_data.current_menu.menu = menu;
			m_data.current_menu.name = name;
		}

		bool menu_manager::unload_current_menu()
		{
			if (m_data.menu_stack.empty())
				return false;
			m_data.current_menu.menu->close();
			m_data.current_menu.menu.reset();
			m_data.current_menu.name.clear();
			return true;
		}
	}
}