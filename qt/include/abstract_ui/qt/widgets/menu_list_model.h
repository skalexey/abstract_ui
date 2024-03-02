#pragma once

#include <QObject>
#include <abstract_ui/qt/widget_model.h>
namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class menu_list_model : public widget_model
			{
				Q_OBJECT
				using base = widget_model;
			};
		}
	}
}
Q_DECLARE_METATYPE(utils::ui::qt::menu_list_model)
