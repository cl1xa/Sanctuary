#include "views/view.hpp"

namespace big
{
	void view_sub::tab_crash()
	{
		ImGui::Checkbox("NET_ARRAY_HANDLER", &g_config.protection.crash.net_array_handler);
		ImGui::Checkbox("SCRIPT_WORLD_STATE", &g_config.protection.crash.script_world_state);
		ImGui::Checkbox("TASK_VEHICLE_TEMP_ACTION", &g_config.protection.crash.task_vehicle_temp_action);
	}
}