#pragma once

namespace big
{
	namespace freemode
	{
		inline void NETWORK_BAIL(rage::scrNativeCallContext* src)
		{
			if (g_config.settings.notify_debug)
				LOG(WARNING) << xorstr_("NETWORK_BAIL TRIGGERED!!!");

			src->set_return_value<BOOL>(FALSE);
		};

		inline void NETWORK_CAN_BAIL(rage::scrNativeCallContext* src)
		{
			if (g_config.settings.notify_debug)
				LOG(WARNING) << xorstr_("NETWORK_CAN_BAIL TRIGGERED!!!");

			src->set_return_value<BOOL>(FALSE);
		}

		//https://github.com/Yimura/YimMenu/discussions/143
		inline void NETWORK_HAS_RECEIVED_HOST_BROADCAST_DATA(rage::scrNativeCallContext* src)
		{
			if (g_config.settings.notify_debug)
				LOG(G3LOG_DEBUG) << xorstr_("NETWORK_HAS_RECEIVED_HOST_BROADCAST_DATA TRIGGERED!!!");

			if (g_config.protection.misc.optimize_loading)
				src->set_return_value<BOOL>(TRUE);
		}
	}
}