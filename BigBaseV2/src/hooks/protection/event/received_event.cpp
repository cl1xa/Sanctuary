#include "gta/enums.hpp"
#include "gta/net_game_event.hpp"
#include "hooking.hpp"

namespace big
{
	void hooks::received_event(rage::netEventMgr* event_manager, CNetGamePlayer* source_player, CNetGamePlayer* target_player, uint16_t event_id, int event_index, int event_handled_bitset, int unk, rage::datBitBuffer* buffer)
	{
		const char* sender_name = source_player->get_name();

		if (g_config.protection.events.event_protocol_cleanup)
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
		}

		switch ((RockstarEvent)event_id)
		{ //Begin switch case

		//Modder detection
		case RockstarEvent::REPORT_CASH_SPAWN_EVENT:
		case RockstarEvent::NETWORK_CHECK_CODE_CRCS_EVENT:
		case RockstarEvent::REPORT_MYSELF_EVENT:
		{
			if (g_config.protection.misc.modder_detection)
				g_notification_service->push_warning(xorstr_("Modder detection"), fmt::format(xorstr_("{} was flagged as a modder for sending unwanted events"), sender_name));

			break; //Pass
		}

		case RockstarEvent::SCRIPT_ENTITY_STATE_CHANGE_EVENT:
		{
			if (g_config.protection.crash.task_vehicle_temp_action)
			{
				uint16_t entity;
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
						g_notification_service->push_warning(xorstr_("Protection"), fmt::format(xorstr_("{} attempted to send TASK_VEHICLE_TEMP_ACTION crash"), sender_name));

						//Send event back to them
						g_pointers->m_send_event_ack(event_manager, source_player, target_player, event_index, event_handled_bitset);

						//Block event
						return;
					}

					buffer->Seek(0);
				}
			}
			break; //Pass
		}

		case RockstarEvent::SCRIPTED_GAME_EVENT:
		{
			if (g_config.protection.events.script)
			{
				const auto scripted_game_event = make_unique<CScriptedGameEvent>();

				buffer->ReadDword(&scripted_game_event->m_args_size, 32);

				if (scripted_game_event->m_args_size - 1 <= 0x1AF)
					buffer->ReadArray(&scripted_game_event->m_args, 8 * scripted_game_event->m_args_size);

				const auto args = scripted_game_event->m_args;
				const auto hash = static_cast<eRemoteEvent>(args[0]);

				//For debugging
				if (g_config.settings.script_event_logger)
				{
					LOG(G3LOG_DEBUG) << xorstr_("===");
					LOG(G3LOG_DEBUG) << fmt::format(xorstr_("PLAYER: {} | EVENT: {}"), source_player->get_name(), int(hash));
					for (size_t i = 1; i < sizeof(args); i++)
						LOG(G3LOG_DEBUG) << fmt::format(xorstr_("Arg #{} : {}"), i, args[i]);
					LOG(G3LOG_DEBUG) << xorstr_("===");
				}

				//If the user sends us an unwanted script event
				if (hooks::scripted_game_event(scripted_game_event.get(), source_player))
				{
					//Send event back to them
					g_pointers->m_send_event_ack(event_manager, source_player, target_player, event_index, event_handled_bitset);

					//Block event
					return;
				}

				buffer->Seek(0);
			}
			break; //Pass

			//Go away
		case RockstarEvent::NETWORK_TRAIN_REPORT_EVENT:
		case RockstarEvent::NETWORK_TRAIN_REQUEST_EVENT:
			return;
		}

		} //End switch case

		//if (g_config.protection.events.game)
		//{
		//	//todo: add game event protection
		//}

		return g_hooking->m_received_event_hook.get_original<decltype(&received_event)>()(event_manager, source_player, target_player, event_id, event_index, event_handled_bitset, unk, buffer);
	}
}