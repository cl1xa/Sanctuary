#include "hooking.hpp"
#include "services/player_service.hpp"
#include "util/notify.hpp"

namespace big
{
	void hooks::player_join(CNetworkObjectMgr* _this, CNetGamePlayer* net_player)
	{
		g_player_service->player_join(net_player);

		if (const rage::netPlayerData* net_player_data = net_player->get_net_data(); net_player_data)
		{
			notify::above_map(fmt::format("<C>{}</C> joined.", net_player->get_name()));

			LOG(INFO) << "Player joined '" << net_player_data->m_name
				<< "' taking slot #" << (int)net_player->m_player_id
				<< " with Rockstar ID: " << net_player_data->m_rockstar_id2;

			if (g_config.settings.notify_players)
				g_notification_service->push("Player Joined", fmt::format("{} taking slot #{} with Rockstar ID: {}", net_player_data->m_name, net_player->m_player_id, net_player_data->m_rockstar_id2));
		}

		return g_hooking->m_player_has_joined_hook.get_original<decltype(&hooks::player_join)>()(_this, net_player);
	}
}