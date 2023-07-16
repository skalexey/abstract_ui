#pragma once

#include <functional>
#include <string>
#include <vl.h>
#include "imgui.h"

namespace ImGui
{
	class AddWidget
	{
	public:
		using on_add_t = std::function<int(const char*)>;

		AddWidget(const std::string& default_value = "") {
			m_value_input.reserve(256);
			m_value_input = default_value;
		}

		void SetOnAdd(const AddWidget::on_add_t& on_add) {
			m_on_add = on_add;
		}
		
		void Show() {
			if (ImGui::Button("Add"))
				if (m_on_add)
					m_on_add(m_value_input.c_str());

			ImGui::SameLine();

			if (ImGui::InputText("##name", m_value_input.data(), m_value_input.capacity()))
				m_value_input.resize(strlen(m_value_input.c_str()));
			
			onShow();
		}

	protected:
		virtual void onShow() {}

	protected:
		std::string m_value_input;
		on_add_t m_on_add;
	};
}