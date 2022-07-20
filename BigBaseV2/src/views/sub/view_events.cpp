#include "views/view.hpp"

namespace big
{
	void view_sub::tab_events()
	{
		ImGui::Checkbox(xorstr_("Block main script"), &g_config.protection.events.block_main_script);
		ImGui::Checkbox(xorstr_("Event protocol cleanup"), &g_config.protection.events.event_protocol_cleanup);
		
		ImGui::Separator();

		ImGui::Checkbox(xorstr_("Block malicious game events"), &g_config.protection.events.game);
		//ImGui::Checkbox(xorstr_("Block malicious script events"), &g_config.protection.events.script);

		ImGui::Separator();

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.0f, 0.2f, 1.f));

		//if (ImGui::IsItemHovered())
		//{
			ImGui::Checkbox(xorstr_("Block control events"), &g_config.protection.events.control);
			ImGui::Checkbox(xorstr_("Block action events"), &g_config.protection.events.action);
		//}
	
		//ImGui::SetTooltip("May cause glitches during regular gameplay");

		ImGui::PopStyleColor();
	}
}