#include "view.hpp"
#include "gui.hpp"
#include "fiber_pool.hpp"
#include "gta/enums.hpp"
#include "gta/session_types.hpp"
#include "util/session.hpp"
#include "pointers.hpp"

namespace big
{
	void view::root()
	{
		static int selected_tab = 0;
		static std::vector<std::string> tabs;

		ImGui::SetNextWindowSize(ImVec2{ 750, 400 }, ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2{ 100, 200 }, ImGuiCond_Once);

		tabs = tabs = { "Kick", "Crash", "Sync", "Misc", "Session", "Players", "Settings"};

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
					//ImGui::Checkbox("Script events", &g_config.protection.kick.script_events);
					//ImGui::Checkbox("Game events", &g_config.protection.kick.game_events);
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
					ImGui::Checkbox("Terminate loading screens", &g_config.protection.misc.terminate_loading_screens);
					ImGui::Checkbox("Rate limit", &g_config.protection.misc.rate_limit);
					ImGui::Checkbox("Modder detection", &g_config.protection.misc.modder_detection);
					ImGui::Checkbox("Optimize session loading", &g_config.protection.misc.optimize_loading);
					ImGui::Checkbox("Event protocol cleanup", &g_config.protection.misc.event_protocol_cleanup);
					break;

				case 4: //Session
					for (const SessionType& session_type : sessions)
					{
						if (ImGui::MenuItem(session_type.name))
						{
							QUEUE_JOB_BEGIN_CLAUSE(&)
							{
								session::join_type(session_type);

							}QUEUE_JOB_END_CLAUSE
						}
					}
					break;

				case 5:
					if (*g_pointers->m_is_session_started)
					{
						if (ImGui::BeginListBox("##plist", { 580 , 350 })) //Gross hardcoded values, but whatever.
						{
							for (auto& item : g_player_service->m_players)
							{
								const auto& plyr = item.second;

								if (plyr->is_host())
									ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.49f, 0.45f, 0.29f, 1.f));

								if (ImGui::Button(plyr->get_name(), { ImGui::GetWindowSize().x - 15.f, 0.f }))
								{
									//g_player_service->set_selected(plyr.get());
								}

								if (plyr->is_host())
									ImGui::PopStyleColor();
							}

							ImGui::EndListBox();
						}
					}
					else
						ImGui::Text("Enter an online session to view the playerlist");
					
					break;

				case 6: //Settings
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