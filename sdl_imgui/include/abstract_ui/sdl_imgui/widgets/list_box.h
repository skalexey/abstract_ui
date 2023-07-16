#pragma once

#include "Variants.h"
#include "imgui.h"

namespace ImGui
{
	class ListBoxWidget : public AbstractVariantsWidget
	{
	public:
		ListBoxWidget(
			const std::string& label = ""
			, const on_select_t& on_select = nullptr
		) : AbstractVariantsWidget(label, on_select) {}

		bool display() override {
			return ImGui::ListBox(m_system_label.c_str(), &m_selected_index, m_variants.data(), m_variants.size(), 4);
		}
	};
}