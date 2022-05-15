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

		tabs = tabs = { "Kick", "Crash", "Sync", "Misc", "Session", "Settings"};

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
				case 0: //Kick
					ImGui::Checkbox("Script events", &g_config.protection.kick.script_events);
					ImGui::Checkbox("Game events", &g_config.protection.kick.game_events);
					//Action events
					//Control events
					break;

				case 1: //Crash
					ImGui::Checkbox("NET_ARRAY_HANDLER", &g_config.protection.crash.net_array_handler);
					ImGui::Checkbox("SCRIPT_WORLD_STATE", &g_config.protection.crash.script_world_state);
					ImGui::Checkbox("TASK_VEHICLE_TEMP_ACTION", &g_config.protection.crash.task_vehicle_temp_action);
					break;

				case 2: //Sync
					ImGui::Text("Not implemented yet");
					break;

				case 3: //Misc
					ImGui::Checkbox("Modder detection", &g_config.protection.misc.modder_detection);
					ImGui::Checkbox("Optimize session loading", &g_config.protection.misc.optimize_loading);
					ImGui::Checkbox("Event protocol cleanup", &g_config.protection.misc.event_protocol_cleanup);
					break;

				case 4: //Session
					ImGui::Text("Not implemented yet");
					break;

				case 5: //Settings
					ImGui::Checkbox("Notify players", &g_config.settings.notify_players);
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