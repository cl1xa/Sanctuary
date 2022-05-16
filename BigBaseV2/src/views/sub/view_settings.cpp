#include "views/view.hpp"

namespace big
{
	void view_sub::tab_settings()
	{
		ImGui::Checkbox(xorstr_("Notify players"), &g_config.settings.notify_players);
		ImGui::Checkbox(xorstr_("Notify debug"), &g_config.settings.notify_debug);

		if (ImGui::Button(xorstr_("Unload")))
			g_running = false;
	}
}