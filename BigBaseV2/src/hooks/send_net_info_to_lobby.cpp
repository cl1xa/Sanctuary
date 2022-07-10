#include "hooking.hpp"

namespace big
{
	bool hooks::send_net_info_to_lobby(rage::netPlayerData* player, int64_t a2, int64_t a3, DWORD* a4)
	{
		/*const bool is_local_player = g_local_player->m_player_info->m_net_player_data.m_rockstar_id == player->m_rockstar_id;

		if (is_local_player)
		{
			if (g->spoofing.spoof_username)
				memcpy(player->m_name, g->spoofing.username.c_str(), sizeof(player->m_name));

			if (g->spoofing.spoof_ip)
			{
				player->m_external_ip.m_field1 = 0;
				player->m_external_ip.m_field2 = 0;
				player->m_external_ip.m_field3 = 0;
				player->m_external_ip.m_field4 = 0;
			}

			if (g->spoofing.spoof_rockstar_id)
			{
				player->m_rockstar_id = 1;
				player->m_rockstar_id2 = 1;
			}
		}

		const auto result = g_hooking->m_send_net_info_to_lobby.get_original<decltype(&hooks::send_net_info_to_lobby)>()(player, a2, a3, a4);

		if (is_local_player && g_config.protection.spoofing)
			memcpy(player->m_name, g_local_player->m_player_info->m_net_player_data.m_name, sizeof(player->m_name));

		return result;*/

		return g_hooking->m_send_net_info_to_lobby.get_original<decltype(&hooks::send_net_info_to_lobby)>()(player, a2, a3, a4);
	}
}