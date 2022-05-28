#include "hooking.hpp"

namespace big
{
	bool hooks::send_net_info_to_lobby(rage::netPlayerData* player, int64_t a2, int64_t a3, DWORD* a4)
	{
		if (g_local_player->m_player_info->m_net_player_data.m_rockstar_id == player->m_rockstar_id)
		{
			/*if (g_config.protections.spoof.ip)
			{
				player->m_external_ip.m_field1 = g->spoofing.ip_address[0];
				player->m_external_ip.m_field2 = g->spoofing.ip_address[1];
				player->m_external_ip.m_field3 = g->spoofing.ip_address[2];
				player->m_external_ip.m_field4 = g->spoofing.ip_address[3];
			}

			if (g_config.protections.spoof.rsid)
			{
				player->m_rockstar_id = g->spoofing.rockstar_id;
				player->m_rockstar_id2 = g->spoofing.rockstar_id;
			}

			LOG(INFO) << xorstr_("Sending spoofed values to session host");
				
			g_notification_service->push(xorstr_("Player Info Spoofing"), xorstr_("Sent spoofed values to lobby host."));*/

			g_notification_service->push(xorstr_("Session Info"), xorstr_("Connected to multiplayer session!"), true);
		}

		return g_hooking->m_send_net_info_to_lobby.get_original<decltype(&hooks::send_net_info_to_lobby)>()(player, a2, a3, a4);
	}
}