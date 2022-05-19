#include "views/view.hpp"
#include "fiber_pool.hpp"
#include "gta_util.hpp"
#include "services/player_service.hpp"

namespace big
{
	void view_sub::tab_settings()
	{
		ImGui::Checkbox(xorstr_("Notify players"), &g_config.settings.notify_players);
		ImGui::Checkbox(xorstr_("Notify debug"), &g_config.settings.notify_debug);

		if (ImGui::Button("NET_BAIL SELF"))
		{
			QUEUE_JOB_BEGIN_CLAUSE()
			{
				NETWORK::NETWORK_BAIL(16, 0, 0);

			} QUEUE_JOB_END_CLAUSE
		}

		if (ImGui::Button(xorstr_("Unload")))
			g_running = false;
	}
}