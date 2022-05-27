#include "view.hpp"
#include "gui.hpp"

namespace big
{
	void view_main::root()
	{
		static size_t selected_tab = 0;
		static vector<string> tabs;

		ImGui::SetNextWindowSize(ImVec2{ 750, 400 }, ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2{ 100, 200 }, ImGuiCond_Once);

		tabs = tabs = { xorstr_("Events"), xorstr_("Crash"), xorstr_("Sync"), xorstr_("Misc"), xorstr_("Network"), xorstr_("Settings") };

		if (ImGui::Begin(xorstr_("Sanctuary"), &g_gui.m_opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
		{
			ImGui::Columns(2);
			{
				ImGui::SetColumnWidth(0, 150.f);

				for (size_t i = 0; i < tabs.size(); i++)
				{
					if (ImGui::Selectable(tabs.at(i).c_str(), selected_tab == i))
						selected_tab = i;
				}

				ImGui::NextColumn();

				switch (selected_tab)
				{
				case 0:
					view_sub::tab_events();
					break;
				case 1:
					view_sub::tab_crash();
					break;
				case 2:
					view_sub::tab_sync();
					break;
				case 3:
					view_sub::tab_misc();
					break;
				case 4:
					view_sub::tab_network();
					break;
				case 5:
					view_sub::tab_settings();
					break;
				}
			}

			ImGui::End();
		}
	}
}