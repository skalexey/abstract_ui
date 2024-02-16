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
		std::unordered_map<ui::app*, menu_manager::menu_manager_data> menu_manager::m_data;

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
			if (this->data().current_menu.menu)
				this->close_current_menu();
			auto& data = this->data();
			auto menu = data.registry.create_menu(name, options);
			assert(menu);
			data.current_menu.menu = menu;
			data.current_menu.name = name;
			data.menu_stack.push_back(name);
		}

		void menu_manager::close_current_menu()
		{
			auto& data = this->data();
			data.current_menu.menu->close();
			data.current_menu.menu.reset();
			data.current_menu.name.clear();
		}

		void menu_manager::close_all_menus()
		{
			while (!this->data().menu_stack.empty())
			{
				this->back();
			}
			if (this->data().current_menu.menu)
			{
				this->close_current_menu();
			}
		}

		void menu_manager::back()
		{
			if (this->data().menu_stack.empty())
			{
				this->close_current_menu();
			}
			else
			{
				auto name = this->data().menu_stack.back();
				this->data().menu_stack.pop_back();
				this->open_menu(name);
			}
		}
	}
}