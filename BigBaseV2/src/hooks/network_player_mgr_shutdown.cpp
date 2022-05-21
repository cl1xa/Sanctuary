#include "hooking.hpp"
#include "services/player_service.hpp"

namespace big
{
	void hooks::network_player_mgr_shutdown(CNetworkPlayerMgr* _this)
	{
		g_player_service->do_cleanup();

		string msg = xorstr_("Leaving session and cleaning up player data");

		LOG(G3LOG_DEBUG) << msg;
		if (g_config.settings.notify_debug)
			g_notification_service->push(xorstr_("Network Player Manager"), msg);

		return g_hooking->m_network_player_mgr_shutdown_hook.get_original<decltype(&hooks::network_player_mgr_shutdown)>()(_this);
	}
}