#include "hooking.hpp"
#include "services/player_service.hpp"

namespace big
{
	void hooks::network_player_mgr_init(CNetworkPlayerMgr* _this, std::uint64_t a2, std::uint32_t a3, std::uint32_t a4[4])
	{
		if (g_config.settings.notify_debug)
		{
			string msg = xorstr_("Entering session and initializing player data");

			LOG(G3LOG_DEBUG) << msg;
			if (g_config.settings.notify_players)
				g_notification_service->push(xorstr_("Network Player Mgr Init"), msg);
		}

		g_hooking->m_network_player_mgr_init_hook.get_original<decltype(&hooks::network_player_mgr_init)>()(_this, a2, a3, a4);

		LOG(G3LOG_DEBUG) << xorstr_("Adding local player to lobby pool...");

		g_player_service->player_join(_this->m_local_net_player);
	}

	void hooks::network_player_mgr_shutdown(CNetworkPlayerMgr* _this)
	{
		g_player_service->do_cleanup();

		if (g_config.settings.notify_debug)
		{
			string msg = xorstr_("Leaving session and cleaning up player data");

			LOG(G3LOG_DEBUG) << msg;
			if (g_config.settings.notify_players)
				g_notification_service->push(xorstr_("Network Player Mgr Shutdown"), msg);
		}

		g_hooking->m_network_player_mgr_shutdown_hook.get_original<decltype(&hooks::network_player_mgr_shutdown)>()(_this);
	}
}