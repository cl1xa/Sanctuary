#pragma once
#include "gta/structs.hpp"

namespace big
{
	namespace freemode
	{
		inline void NETWORK_BAIL(rage::scrNativeCallContext* src)
		{
			if (g_config.settings.notify_debug)
				g_notification_service->push(xorstr_("NETWORK_BAIL"), xorstr_("!NETWORK_BAIL TRIGGERED!"), true);

			src->set_return_value<BOOL>(FALSE);
		};

		inline void NETWORK_CAN_BAIL(rage::scrNativeCallContext* src)
		{
			if (g_config.settings.notify_debug)
				g_notification_service->push(xorstr_("NETWORK_CAN_BAIL"), xorstr_("!NETWORK_CAN_BAIL TRIGGERED!"), true);

			src->set_return_value<BOOL>(FALSE);
		}

		//https://github.com/Yimura/YimMenu/discussions/143
		inline void NETWORK_HAS_RECEIVED_HOST_BROADCAST_DATA(rage::scrNativeCallContext* src)
		{
			LOG(G3LOG_DEBUG) << xorstr_("!NETWORK_HAS_RECEIVED_HOST_BROADCAST_DATA TRIGGERED!");

			if (g_config.protection.misc.optimize_loading)
				src->set_return_value<BOOL>(TRUE);
		}

		inline void REQUEST_SCRIPT(rage::scrNativeCallContext* src)
		{
			const char* script_name = src->get_arg<const char*>(0);

			if (g_config.protection.events.block_main_script)
			{
				for (int a = 0; a < sizeof(blocked_scripts) / sizeof(blocked_scripts[0]); a++)
				{
					if (blocked_scripts[a] == script_name)
					{
						g_notification_service->push_warning(xorstr_("REQUEST_SCRIPT"), fmt::format("Blocked main script: {} from triggering", blocked_scripts[a]));

						src->set_return_value<BOOL>(FALSE);
						return; //Prevent the code below from running
					}
				}
			}

			//LOG(INFO) << fmt::format(xorstr_("Requested script: {}"), script_name);
			SCRIPT::REQUEST_SCRIPT(script_name);
		}
	}
}