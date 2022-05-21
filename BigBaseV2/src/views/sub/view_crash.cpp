#include "views/view.hpp"

namespace big
{
	void view_sub::tab_crash()
	{
		ImGui::Checkbox(xorstr_("NET_ARRAY_HANDLER patch"), &g_config.protection.crash.net_array_handler);
		ImGui::Checkbox(xorstr_("SCRIPT_WORLD_STATE patch"), &g_config.protection.crash.script_world_state);
		ImGui::Checkbox(xorstr_("TASK_VEHICLE_TEMP_ACTION patch"), &g_config.protection.crash.task_vehicle_temp_action);
	}
}