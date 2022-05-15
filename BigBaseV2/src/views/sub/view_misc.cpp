#include "views/view.hpp"

namespace big
{
	void view_sub::tab_misc()
	{
		ImGui::Checkbox("Terminate loading screens", &g_config.protection.misc.terminate_loading_screens);
		ImGui::Checkbox("Rate limit", &g_config.protection.misc.rate_limit);
		ImGui::Checkbox("Modder detection", &g_config.protection.misc.modder_detection);
		ImGui::Checkbox("Optimize session loading", &g_config.protection.misc.optimize_loading);
		ImGui::Checkbox("Event protocol cleanup", &g_config.protection.misc.event_protocol_cleanup);
	}
}