#include "views/view.hpp"
#include "gta/enums.hpp"
#include "util/session.hpp"
#include "fiber_pool.hpp"

namespace big
{
	void view_sub::tab_network()
	{
		if (ImGui::BeginTabBar(xorstr_("##network_tabbar")))
		{
			if (ImGui::BeginTabItem(xorstr_("Session")))
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

			if (ImGui::BeginTabItem(xorstr_("Players")))
			{
				if (*g_pointers->m_is_session_started)
				{
					const auto player_count = g_player_service->players().size() + 1; // +1 is you

					ImGui::Text(fmt::format(xorstr_("Players in session: {}"), g_player_service->players().size() + 1).c_str());

					if (ImGui::BeginListBox("##players", { 350.f - ImGui::GetStyle().WindowPadding.x * 2 , 285 }))
					{
						ImGui::Text(g_player_service->get_self()->get_name());

						ImGui::Separator();

						for (const auto& [_, player] : g_player_service->players())
						{
							ImGui::PushID(player->id());
							ImGui::Text(player->get_name());
							ImGui::PopID();
						}

						ImGui::EndListBox();
					}
				}
				else
					ImGui::Text(xorstr_("GTA Online is required in order to view the playerlist"));

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
}