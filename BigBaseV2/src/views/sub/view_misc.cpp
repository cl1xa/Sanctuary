#include "views/view.hpp"

namespace big
{
	void view_sub::tab_misc()
	{
		ImGui::Checkbox(xorstr_("Idle kick prevention"), &g_config.protection.misc.idle);
		ImGui::Checkbox(xorstr_("Purge duplicate chat messages"), &g_config.protection.misc.chat_spam);
		ImGui::Checkbox(xorstr_("Terminate loading screens"), &g_config.protection.misc.terminate_loading_screens);
		ImGui::Checkbox(xorstr_("Rate limit prevention"), &g_config.protection.misc.rate_limit);
		ImGui::Checkbox(xorstr_("Modder detection"), &g_config.protection.misc.modder_detection);
		ImGui::Checkbox(xorstr_("Optimize session loading"), &g_config.protection.misc.optimize_loading);
	}
}