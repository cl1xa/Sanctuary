#include "views/view.hpp"

namespace big
{
	void view_sub::tab_kick()
	{
		ImGui::Checkbox(xorstr_("Script event handler"), &g_config.protection.kick.script_events);
		//ImGui::Checkbox("Scripted game event protection", &g_config.protection.kick.game_events);
		ImGui::Checkbox(xorstr_("Host desync prevention"), &g_config.protection.kick.desync);
		//Action events
		//Control events
	}
}