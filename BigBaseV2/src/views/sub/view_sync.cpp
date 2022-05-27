#include "views/view.hpp"

namespace big
{
	void view_sub::tab_sync()
	{
		ImGui::Text(xorstr_("Object"));
		ImGui::Checkbox(xorstr_("Clone sync"), &g_config.protection.sync.clone_sync);

		ImGui::Separator();
		
		ImGui::Text(xorstr_("Network"));
		ImGui::Checkbox(xorstr_("Host desync prevention"), &g_config.protection.sync.host_desync);
	}
}