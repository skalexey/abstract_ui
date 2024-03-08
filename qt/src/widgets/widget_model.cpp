#pragma once

#include <utils/log.h>
#include <abstract_ui/qt/widget_model.h>

LOG_TITLE("qt::widget_model");

#define CHECK_PROPERTY(name) \
	assert(m_##name.isValid()); \
	if (!m_##name.isValid()) \
	{ \
		LOG_WARNING("Invalid property '"#name"'"); \
		return; \
	}

#define INIT_AND_CHECK_RPOPERTY(object, name) \
	m_##name = QQmlProperty(object, ""#name""); \
	CHECK_PROPERTY(name)

#define STORE_PROPERTY_CHANGE(name) \
	vl::Object info; \
	auto value = m_##name.read().toInt(); \
	m_update_info.Set(""#name"", value); \
	LOG_DEBUG(this << ": "#name"Changed to " << value); \

namespace utils
{
	namespace ui
	{
		namespace qt
        {
			void widget_model::connect_to_node(qt::node* node)
			{
				assert(node);
				m_node = node;
				auto object = m_node->qobject();
				assert(object);
				setParent(object);

				INIT_AND_CHECK_RPOPERTY(object, x);
				m_x.connectNotifySignal(this, SLOT(xChanged()));
					
				INIT_AND_CHECK_RPOPERTY(object, y);
				m_y.connectNotifySignal(this, SLOT(yChanged()));

				INIT_AND_CHECK_RPOPERTY(object, width);
				m_width.connectNotifySignal(this, SLOT(widthChanged()));

				INIT_AND_CHECK_RPOPERTY(object, height);
				m_height.connectNotifySignal(this, SLOT(heightChanged()));
			}

			vl::VarPtr widget_model::grab_update()
			{
				if (m_update_info.IsEmpty())
					return vl::VarPtr(nullptr);
				auto info_ptr = vl::MakePtr(m_update_info);
				m_update_info = vl::Object();
				return info_ptr;
			}

            void widget_model::xChanged()
			{
				STORE_PROPERTY_CHANGE(x);
			}

			void widget_model::yChanged()
			{
				STORE_PROPERTY_CHANGE(y);
			}

			void widget_model::widthChanged()
			{
				STORE_PROPERTY_CHANGE(width);
			}

			void widget_model::heightChanged()
			{
				STORE_PROPERTY_CHANGE(height);
			}
		}
	}
}
