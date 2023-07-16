#pragma once

#include <abstract_ui/qt/widget_factory.h>

// It is initialized in instance() method because of early access-time by users.
utils::ui::qt::widget_factory::creators_t* utils::ui::qt::widget_factory::m_creators = nullptr;

utils::ui::qt::widget_factory& utils::ui::qt::widget_factory::instance()
{
	static widget_factory instance;
	return instance;
}
