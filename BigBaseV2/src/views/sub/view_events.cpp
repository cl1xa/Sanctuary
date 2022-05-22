#include "views/view.hpp"

namespace big
{
	void view_sub::tab_events()
	{
		ImGui::Checkbox(xorstr_("Block main script"), &g_config.protection.events.block_main_script);
		ImGui::Checkbox(xorstr_("Event protocol cleanup"), &g_config.protection.events.event_protocol_cleanup);
		//ImGui::Checkbox(xorstr_("Script event handler"), &g_config.protection.events.script);
		//ImGui::Checkbox("Scripted game event protection", &g_config.protection.kick.game_events);
		//Action events
		//Control events
	}
}