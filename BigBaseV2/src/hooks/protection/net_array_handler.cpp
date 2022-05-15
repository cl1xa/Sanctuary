#include "hooking.hpp"

namespace big
{
	bool hooks::net_array_handler(long long netArrayHandlerBaseMgr, CNetGamePlayer* a2, rage::datBitBuffer* datbitbuffer, unsigned int bytes_to_read, short a5)
	{
		/*if (datbitbuffer->m_bitsRead + bytes_to_read > datbitbuffer->m_curBit)
		{
			LOG(WARNING) << "Received NET_ARRAY_ERROR crash from " << a2->get_name();
			g_notification_service->push_warning("Protections", fmt::format("Detected NET_ARRAY_ERROR crash from {}", a2->get_name()));

			return false;
		}*/

		return g_hooking->m_net_array_handler_hook.get_original<decltype(&hooks::net_array_handler)>()(netArrayHandlerBaseMgr, a2, datbitbuffer, bytes_to_read, a5);
	}
}