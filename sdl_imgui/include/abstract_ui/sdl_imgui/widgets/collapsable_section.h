#pragma once

#include <string>
#include "imgui.h"

namespace ImGui
{
	class CollapsableSectionWidget
	{
		public:
			CollapsableSectionWidget(const std::string& title) : m_title(title) {}

			void SetTitle(const std::string& title) { m_title = title; }
			const std::string& GetTitle() { return m_title; }

			virtual void Show() {
				if (m_title.empty())
					return;
				if (ImGui::CollapsingHeader(m_title.c_str()))
				{
					ImGui::PushID(int(this));
					OnExpanded();
					ImGui::PopID();
				}
				else
				{
					ImGui::PushID(int(this));
					OnCollapsed();
					ImGui::PopID();
				}
			}
			virtual void OnExpanded() {}
			virtual void OnCollapsed() {}

		private:
			std::string m_title;
	};
}