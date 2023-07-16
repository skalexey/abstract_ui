#pragma once

#include "ValueInput.h"

namespace ImGui
{
	class FloatInputWidget : public AbstractValueInputWidget<float>
	{
	public:
		FloatInputWidget(
			const std::string& label
			, float default_value
			, const on_update_t& on_update = nullptr
		)
			: AbstractValueInputWidget(label, default_value, on_update)
		{}

	protected:
		bool display() override {
			return ImGui::InputFloat(m_input_label.c_str(), &m_edit_value, 0.01f, 0.1f);
		}
	};
}