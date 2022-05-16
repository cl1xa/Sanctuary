#include "views/view.hpp"

namespace big
{
	void view_sub::tab_kick()
	{
		ImGui::Checkbox(xorstr_("Script events"), &g_config.protection.kick.script_events);
		//ImGui::Checkbox("Game events", &g_config.protection.kick.game_events);
		//Action events
		//Control events
	}
}