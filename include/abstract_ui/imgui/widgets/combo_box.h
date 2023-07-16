#pragma once

#include "Variants.h"
#include "imgui.h"

namespace ImGui
{
	class ComboBoxWidget : public AbstractVariantsWidget
	{
	public:
		ComboBoxWidget(
			const std::string& label = ""
			, const on_select_t& on_select = nullptr
		) : AbstractVariantsWidget(label, on_select) {}

		bool display() override {
			return ImGui::Combo(m_system_label.c_str(), &m_selected_index, m_variants.data(), m_variants.size(), 4);
		}
	};
}