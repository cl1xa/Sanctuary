#include "hooking.hpp"

namespace big
{
	signed __int64 hooks::received_clone_sync(CNetworkObjectMgr* mgr, CNetGamePlayer* src, CNetGamePlayer* dst, unsigned __int16 sync_type, unsigned __int16 obj_id, rage::datBitBuffer* buffer, unsigned __int16 a7, unsigned int timestamp)
	{
		if (g_config.protection.sync.clone_sync)
		{
			auto sync_tree = g_pointers->m_get_sync_tree_for_type(mgr, sync_type);
			auto tree_name = g_pointers->m_get_sync_type_info(sync_type, 0);
			auto net_obj = g_pointers->m_get_net_object(mgr, obj_id, true);
			bool invalidsync = false;

			if (!net_obj)
				net_obj = g_pointers->m_get_net_object_for_player(mgr, obj_id, src, true);

			if (!net_obj)
				return 2;

			if (!sync_tree || sync_type < 0 || sync_type > 14)
				invalidsync = true;

			if (net_obj->m_object_type != sync_type)
				invalidsync = true;

			if (invalidsync)
			{
				g_notification_service->push_warning(xorstr_("Protection"), fmt::format("{} sent invalid sync type: {} | Type name: {}", std::string(src->get_name()), std::to_string(sync_type), tree_name));

				return 2;
			}
		}

		auto result = g_hooking->m_received_clone_sync_hook.get_original<decltype(&received_clone_sync)>()(mgr, src, dst, sync_type, obj_id, buffer, a7, timestamp);

		return result;
	}
}