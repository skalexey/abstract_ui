#pragma once

#include <string>
#include <functional>
#include <utility>
#include <utils/string_utils.h>
#include "ImGuiUtils.h"

namespace ImGui
{
	class AbstractVariantsWidget
	{
	public:
		using on_select_t = std::function<void(int, const char*)>;
		AbstractVariantsWidget(
			const std::string& label = ""
			, const on_select_t& on_select = nullptr
		)
			: m_label(label) 
			, m_system_label(utils::format_str("##%s", m_label.c_str()))
			, m_on_select(on_select)
		{}

		void SetSelectedIndex(int index) { m_selected_index = index; }
		int SelectedIndex() { return m_selected_index; }
		void SetVariants(string_list_t&& variants) { m_variants = std::move(variants); }
		void SetOnSelect(const on_select_t& on_select) { m_on_select = on_select; }
		void OnSelect() const {
			if (m_on_select)
				m_on_select(
					m_selected_index
					, (m_selected_index >= 0 && m_selected_index < m_variants.size())
						? m_variants[m_selected_index]
						: nullptr
				);
		}

		bool Show() {
			if (display()) {
				OnSelect();
				return true;
			}
			return false;
		}

	protected:
		virtual bool display() = 0;

	protected:
		string_list_t m_variants;
		on_select_t m_on_select;
		std::string m_label;
		std::string m_system_label;
		int m_selected_index = -1;
	};
}