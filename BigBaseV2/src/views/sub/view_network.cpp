#include "views/view.hpp"
#include "gta/enums.hpp"
#include "util/session.hpp"
#include "fiber_pool.hpp"

namespace big
{
	void view_sub::tab_network()
	{
		if (*g_pointers->m_is_session_started)
		{
			if (ImGui::BeginTabBar("##network_tabbar"))
			{
				if (ImGui::BeginTabItem("Session"))
				{
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

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Players"))
				{
					if (ImGui::BeginListBox("##plist", { 580 , 315 })) //Gross hardcoded values, but whatever.
					{
						for (auto& item : g_player_service->m_players)
						{
							const auto& plyr = item.second;
							const char* who = plyr->get_name();

							if (plyr->is_host())
								ImGui::TextColored(ImVec4(1.000f, 0.879f, 0.000f, 1.000f), who);
							else if (plyr->is_friend())
								ImGui::TextColored(ImVec4(0.365f, 0.714f, 0.898f, 1.000f), who);
							else
								ImGui::Text(who);
						}

						ImGui::EndListBox();
					}

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}
		else
			ImGui::Text("GTA V Online is required in order to uses these features");
	}
}