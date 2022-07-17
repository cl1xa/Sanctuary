#include "gta/joaat.hpp"
#include "hooking.hpp"
#include "script_global.hpp"

namespace big
{
	string previous_message = "";

	__int64* hooks::chat_receive(__int64 chat_pointer, __int64 unk2, __int64 peerId, const char* msg, char IsTeam)
	{
		if (g_config.protection.misc.chat_spam)
		{
			//Duplicate chat message spam
			if (msg == previous_message) 
			{
				g_notification_service->push_warning(xorstr_("Protections"), fmt::format(xorstr_("Duplicate chat message purged: {}"), msg));

				return 0;
			}

			previous_message = msg;
		}

		return g_hooking->m_chat_receive_hook.get_original<decltype(&chat_receive)>()(chat_pointer, unk2, peerId, msg, IsTeam);
	}
}