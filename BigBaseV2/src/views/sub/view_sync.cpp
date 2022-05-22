#include "views/view.hpp"

namespace big
{
	void view_sub::tab_sync()
	{
		ImGui::Checkbox(xorstr_("Host desync prevention"), &g_config.protection.sync.host_desync);
		ImGui::Separator();
		ImGui::Text(xorstr_("Object sync patches not implemented yet"));
	}
}