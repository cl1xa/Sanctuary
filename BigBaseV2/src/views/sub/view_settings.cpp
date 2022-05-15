#include "views/view.hpp"

namespace big
{
	void view_sub::tab_settings()
	{
		ImGui::Checkbox("Notify players", &g_config.settings.notify_players);
		ImGui::Checkbox("Notify debug", &g_config.settings.notify_debug);

		if (ImGui::Button("Unload"))
			g_running = false;
	}
}