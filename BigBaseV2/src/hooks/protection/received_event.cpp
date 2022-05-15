#include "gta/enums.hpp"
#include "gta/net_game_event.hpp"
#include "hooking.hpp"

namespace big
{
	void hooks::received_event(
		rage::netEventMgr* event_manager,
		CNetGamePlayer* source_player,
		CNetGamePlayer* target_player,
		uint16_t event_id,
		int event_index,
		int event_handled_bitset,
		int unk,
		rage::datBitBuffer* buffer
	)
	{
		if (event_id > 91u)
		{
			g_pointers->m_send_event_ack(event_manager, source_player, target_player, event_index, event_handled_bitset);

			return;
		}

		const auto event_name = *(char**)((DWORD64)event_manager + 8i64 * event_id + 243376);
		if (event_name == nullptr || source_player == nullptr || source_player->m_player_id < 0 || source_player->m_player_id >= 32)
		{
			g_pointers->m_send_event_ack(event_manager, source_player, target_player, event_index, event_handled_bitset);
			return;
		}

		switch ((RockstarEvent)event_id)
		{
		case RockstarEvent::SCRIPT_ENTITY_STATE_CHANGE_EVENT:
		{
			/*uint16_t entity;
			buffer->ReadWord(&entity, 13);
			uint32_t type;
			buffer->ReadDword(&type, 4);
			uint32_t unk;
			buffer->ReadDword(&unk, 32);

			if (type == 6)
			{
				uint16_t unk2;
				buffer->ReadWord(&unk2, 13);
				uint32_t action;
				buffer->ReadDword(&action, 8);

				if (action >= 16 && action <= 18)
				{
					g_pointers->m_send_event_ack(event_manager, source_player, target_player, event_index, event_handled_bitset);

					LOG(INFO) << "RECEIVED_EVENT_HANDLER : " << source_player->get_name() << "sent TASK_VEHICLE_TEMP_ACTION crash.";

					g_notification_service->push_warning("Protection",
						fmt::format("{} sent TASK_VEHICLE_TEMP_ACTION crash.", source_player->get_name()));

					return;
				}
			}

			buffer->Seek(0);*/
			break;
		}
		case RockstarEvent::SCRIPTED_GAME_EVENT:
		{
			/*const auto scripted_game_event = std::make_unique<CScriptedGameEvent>();

			buffer->ReadDword(&scripted_game_event->m_args_size, 32);

			if (scripted_game_event->m_args_size - 1 <= 0x1AF)
				buffer->ReadArray(&scripted_game_event->m_args, 8 * scripted_game_event->m_args_size);

			if (hooks::scripted_game_event(scripted_game_event.get(), source_player))
			{
				g_pointers->m_send_event_ack(event_manager, source_player, target_player, event_index, event_handled_bitset);

				return;
			}

			buffer->Seek(0);*/
			break;
		}

		//case RockstarEvent::NETWORK_CLEAR_PED_TASKS_EVENT:
		//{
		//	if (source_player->m_player_id < 32)
		//	{
		//		g_pointers->m_send_event_ack(event_manager, source_player, target_player, event_index, event_handled_bitset);

		//		LOG(INFO) << "RECEIVED_EVENT_HANDLER : " << source_player->get_name() << " sent CLEAR_PED_TASKS event.";

		//		g_notification_service->push_warning("Protection",
		//			fmt::format("{} possible attempt at freezing entity.", source_player->get_name())
		//		);

		//		return;
		//	}

		//	break;
		//}
		//// Don't block this event, we still want to report this player
		//// because if we still report others, our account seems less fishy
		//case RockstarEvent::REPORT_CASH_SPAWN_EVENT:
		//{
		//	uint32_t money;

		//	buffer->Seek(64);
		//	buffer->ReadDword(&money, 32);
		//	buffer->Seek(0);

		//	if (money >= 2000)
		//	{
		//		LOG(INFO) << "RECEIVED_EVENT_HANDLER : " << source_player->get_name() << " sent REPORT_CASH_SPAWN event.";

		//		g_notification_service->push_warning("Protection",
		//			fmt::format("{} is spawning cash.", source_player->get_name())
		//		);
		//	}

		//	break;
		//}
		//// player sending this event is a modder
		//case RockstarEvent::NETWORK_CHECK_CODE_CRCS_EVENT:
		//case RockstarEvent::REPORT_MYSELF_EVENT:
		//{
		//	LOG(INFO) << "RECEIVED_EVENT_HANDLER : " << source_player->get_name() << " sent modder event.";

		//	g_notification_service->push_warning("Protection",
		//		fmt::format("Detected {} as cheating.", source_player->get_name())
		//	);

		//	break;
		//}
		//case RockstarEvent::REQUEST_CONTROL_EVENT:
		//{
		//	g_pointers->m_send_event_ack(event_manager, source_player, target_player, event_index, event_handled_bitset);

		//	LOG(INFO) << "RECEIVED_EVENT_HANDLER : " << source_player->get_name() << " sent modder event.";

		//	g_notification_service->push_warning("Protection",
		//		fmt::format("Denied player control request from {}", source_player->get_name())
		//	);

		//	return;
		//}
		}

		return g_hooking->m_received_event_hook.get_original<decltype(&received_event)>()(event_manager, source_player, target_player, event_id, event_index, event_handled_bitset, unk, buffer);
	}
}