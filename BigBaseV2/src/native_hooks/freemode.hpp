#pragma once

namespace big
{
	namespace freemode
	{
		inline void NETWORK_BAIL(rage::scrNativeCallContext* src)
		{
			if (g_config.settings.notify_debug)
			{
				string msg = xorstr_("!NETWORK_BAIL TRIGGERED!");

				LOG(G3LOG_DEBUG) << msg;
				if (g_config.settings.notify_debug)
					g_notification_service->push_warning(xorstr_("NETWORK_BAIL"), msg);
			}

			src->set_return_value<BOOL>(FALSE);
		};

		inline void NETWORK_CAN_BAIL(rage::scrNativeCallContext* src)
		{
			if (g_config.settings.notify_debug)
			{
				string msg = xorstr_("!NETWORK_CAN_BAIL TRIGGERED!");

				LOG(G3LOG_DEBUG) << msg;
				if (g_config.settings.notify_debug)
					g_notification_service->push_warning(xorstr_("NETWORK_CAN_BAIL"), msg);
			}

			src->set_return_value<BOOL>(FALSE);
		}

		//https://github.com/Yimura/YimMenu/discussions/143
		inline void NETWORK_HAS_RECEIVED_HOST_BROADCAST_DATA(rage::scrNativeCallContext* src)
		{
			string msg = xorstr_("!NETWORK_HAS_RECEIVED_HOST_BROADCAST_DATA TRIGGERED!");

			LOG(G3LOG_DEBUG) << msg;

			if (g_config.protection.misc.optimize_loading)
				src->set_return_value<BOOL>(TRUE);
		}

		inline void REQUEST_SCRIPT(rage::scrNativeCallContext* src)
		{
			const char* script_name = src->get_arg<const char*>(0);

			if (g_config.protection.events.block_main_script)
			{
				const std::string blocked_script[] = { "DISABLED-MainTransition", "DISABLED-cellphone_controller" };

				for (int a = 0; a < sizeof(blocked_script) / sizeof(blocked_script[0]); a++)
				{
					if (blocked_script[a] == script_name)
					{
						string msg = fmt::format("Blocked main script: {} from triggering", blocked_script[a]);

						LOG(G3LOG_DEBUG) << msg;
						if (g_config.settings.notify_debug)
							g_notification_service->push_warning(xorstr_("REQUEST_SCRIPT"), msg);

						src->set_return_value<BOOL>(FALSE);
						return; //Prevent the code below from running
					}
				}
			}

			LOG(INFO) << fmt::format(xorstr_("Requested script: {}"), script_name);
			SCRIPT::REQUEST_SCRIPT(script_name);
		}
	}
}