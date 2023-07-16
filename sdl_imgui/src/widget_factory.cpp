#pragma once

#include <abstract_ui/imgui/widget_factory.h>

// It is initialized in instance() method because of early access-time by users.
utils::ui::imgui::widget_factory::creators_t* utils::ui::imgui::widget_factory::m_creators = nullptr;

utils::ui::imgui::widget_factory& utils::ui::imgui::widget_factory::instance()
{
	static widget_factory instance;
	return instance;
}
