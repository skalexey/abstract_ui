#pragma once

#include <string>
#include <functional>
#include <utils/string_utils.h>
#include "imgui.h"

namespace ImGui
{
	class SliderFloatWidget
	{
	public:
		using on_update_t = std::function<void(float)>;
		SliderFloatWidget(
			const std::string& label
			, float default_value
			, float min = 0.f
			, float max = 1.f
		)
			: m_edit_value(default_value)
			, m_label(label)
			, m_input_label(utils::format_str("##%s", m_label.c_str()))
			, m_min(min)
			, m_max(max)
		{}

		void SetOnUpdate(const on_update_t& on_update) {
			m_on_update = on_update;
		}
		void OnUpdate() const {
			if (m_on_update)
				m_on_update(m_edit_value);
		}
		void OnUpdate(float value) {
			if (m_on_update)
				m_on_update(m_edit_value = value);
		}

		bool Show() {
			ImGui::Text("%s", m_label.c_str());
			ImGui::SameLine();
			if (ImGui::SliderFloat(m_input_label.c_str(), &m_edit_value, m_min, m_max))
			{
				OnUpdate();
				return true;
			}
			return false;
		}

		float Value() const { return m_edit_value; }
		void SetValue(float value) { m_edit_value = value; }
		float Min() const { return m_min; }
		void SetMin(float value) { m_min = value; }
		float Max() const { return m_max; }
		void SetMax(float value) { m_max = value; }

	private:
		on_update_t m_on_update;
		float m_edit_value = -1.f;
		float m_min = -1.f;
		float m_max = -1.f;
		std::string m_label;
		std::string m_input_label;
	};
}