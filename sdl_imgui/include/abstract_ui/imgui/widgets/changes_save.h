#pragma once

#include <functional>
#include "imgui.h"

namespace ImGui
{
	class ChangesSaveWidget
	{
	public:
		using cb_t = std::function<void()>;

		ChangesSaveWidget() {}

		void SetOnSave(const cb_t& on_save) {
			m_on_save = on_save;
		}

		void SetOnCancel(const cb_t& on_cancel) {
			m_on_cancel = on_cancel;
		}

		bool Changed() {
			return m_changed;
		}

		void SetChanged(bool changed) {
			m_changed = changed;
		}

		void Show() {
			if (m_changed)
			{
				if (ImGui::Button("Save"))
					if (m_on_save)
						m_on_save();

				ImGui::SameLine();

				if (ImGui::Button("Undo"))
					if (m_on_cancel)
						m_on_cancel();
			}
		}

	private:
		cb_t m_on_cancel;
		cb_t m_on_save;
		bool m_changed = false;
	};
}