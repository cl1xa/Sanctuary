#pragma once

namespace big
{
	namespace freemode
	{
		inline void NETWORK_BAIL(rage::scrNativeCallContext* src)
		{
			LOG(WARNING) << "NETWORK_BAIL TRIGGERED!!!";

			src->set_return_value<BOOL>(FALSE);
		};

		inline void NETWORK_CAN_BAIL(rage::scrNativeCallContext* src)
		{
			LOG(WARNING) << "NETWORK_CAN_BAIL TRIGGERED!!!";

			src->set_return_value<BOOL>(FALSE);
		}

		//https://github.com/Yimura/YimMenu/discussions/143
		inline void NETWORK_HAS_RECEIVED_HOST_BROADCAST_DATA(rage::scrNativeCallContext* src)
		{
			LOG(G3LOG_DEBUG) << "NETWORK_HAS_RECEIVED_HOST_BROADCAST_DATA TRIGGERED!!!";

			src->set_return_value<BOOL>(TRUE);
		}
	}
}