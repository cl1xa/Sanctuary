#include "view.hpp"
#include "gui.hpp"

namespace big
{
	void view::root()
	{
		static int selected_tab = 0;
		static std::vector<std::string> tabs;

		ImGui::SetNextWindowSize(ImVec2{ 750, 400 }, ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2{ 100, 200 }, ImGuiCond_Once);

		tabs = tabs = { "Network Events", "Script Events", "Object Sync", "Crash", "Misc", "Session", "Settings" };

		if (ImGui::Begin("Sanctuary", &g_gui.m_opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
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

				//TODO: Do this properly lol, for now its just here as filler
				switch (selected_tab)
				{
				case 0:
					ImGui::Text("1");
					break;
				case 1:
					ImGui::Text("2");
					break;
				case 2:
					ImGui::Text("3");
					break;
				case 3:
					ImGui::Text("4");
					break;
				case 4:
					ImGui::Text("5");
					break;
				case 5:
					ImGui::Text("6");
					break;
				case 6:
					ImGui::Checkbox("Optimize session loading", &g_config.settings.optimize_loading);
					ImGui::Checkbox("Notify players", &g_config.settings.notify_players);//Currently this only toggles one thing, but there is more that I can do.
					ImGui::Checkbox("Notify debug", &g_config.settings.notify_debug);

					if (ImGui::Button("Unload"))
						g_running = false;

					break;
				}
			}
			ImGui::End();
		}
	}
}