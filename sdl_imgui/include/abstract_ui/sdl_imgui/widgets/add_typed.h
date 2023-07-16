#pragma once

#include <functional>
#include <string>
#include <vl.h>
#include "imgui.h"
#include "ImGuiUtils.h"
#include "Add.h"

namespace ImGui
{
	class AddTypedWidget : public AddWidget
	{
	public:
		using variants_getter_t = std::function<vl::List()>;
		using on_add_t = std::function<int(const char*, const char*)>;

		AddTypedWidget(const variants_getter_t& variants_getter, const std::string& default_value)
			: AddWidget(default_value)
			, m_get_variants(variants_getter) 
		{}

		void SetOnAdd(const AddTypedWidget::on_add_t& on_add) {
			m_on_add = on_add;
			AddWidget::SetOnAdd([&](const char* object_name) {
				if (m_on_add)
					return m_on_add(object_name, getSelectedType());
				return -1;
			});
		}

	protected:
		void onShow() override {
			ImGui::SameLine();
			auto variants = getVariants();
			ImGui::Combo("##variants", &m_selected_index, variants.data(), variants.size());
		}

		const char* getSelectedType() {
			auto list = m_get_variants();
			if (m_selected_index >= 0 && m_selected_index < list.Size())
				return list.At(m_selected_index).AsString().Val().c_str();
			return nullptr;
		}

		string_list_t getVariants() {
			string_list_t variants;
			auto list = m_get_variants();
			variants.reserve(list.Size());
			for (int i = 0; i < list.Size(); i++)
				variants.push_back(list.At(i).AsString().Val().c_str());
			return variants;
		}

	protected:
		int m_selected_index = 0;
		on_add_t m_on_add;
		variants_getter_t m_get_variants;
	};
}