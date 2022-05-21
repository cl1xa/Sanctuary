#include "gta/joaat.hpp"
#include "hooking.hpp"
#include "natives.hpp"
#include "script_global.hpp"
#include "util/find.hpp"

namespace big
{
	std::string previous_message = "";

	__int64* hooks::chat_receive(__int64 chat_pointer, __int64 unk2, __int64 peerId, const char* msg, char IsTeam)
	{
		if (g_config.protection.misc.chat_spam)
		{
			//Duplicate chat message spam
			if (msg == previous_message) 
			{
				string it = fmt::format(xorstr_("Duplicate chat message purged: {}"), msg);

				LOG(WARNING) << it;
				g_notification_service->push_warning(xorstr_("Protections"), it);

				return nullptr;
			}

			previous_message = msg;
		}

		return g_hooking->m_chat_receive_hook.get_original<decltype(&chat_receive)>()(chat_pointer, unk2, peerId, msg, IsTeam);
	}
}