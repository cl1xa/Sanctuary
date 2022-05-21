#include "gta/enums.hpp"
#include "gta/net_game_event.hpp"
#include "hooking.hpp"

namespace big
{
	void hooks::received_event(rage::netEventMgr* event_manager, CNetGamePlayer* source_player, CNetGamePlayer* target_player, uint16_t event_id, int event_index, int event_handled_bitset, int unk, rage::datBitBuffer* buffer)
	{
		const char* sender_name = source_player->get_name();

		//This looks like garbage but it works... I will clean this up in the future.
		if (g_config.protection.misc.event_protocol_cleanup)
		{
			const auto event_name = *(char**)((DWORD64)event_manager + 8i64 * event_id + 243376);

			//This shouldn't ever happen, but if it does we can catch it.
			if (event_manager == nullptr || event_name == nullptr || source_player == nullptr || target_player == nullptr || buffer == nullptr)
			{
				string msg = fmt::format(xorstr_("Malformed protocol information from: {} | {}"), sender_name, event_name);

				LOG(WARNING) << msg;
				if (g_config.settings.notify_debug)
					g_notification_service->push_warning(xorstr_("Event Protocol"), msg);

				return;
			}

			if (event_id > 91u || source_player->m_player_id < 0 || source_player->m_player_id >= 32)
			{
				switch ((RockstarEvent)event_id)
				{
					//Blocking these can either crash you or cause weird issues
				case RockstarEvent::REMOTE_SCRIPT_INFO_EVENT:
				case RockstarEvent::REMOTE_SCRIPT_LEAVE_EVENT:
				case RockstarEvent::NETWORK_CHECK_EXE_SIZE_EVENT:
				case RockstarEvent::NETWORK_GARAGE_OCCUPIED_STATUS_EVENT:
					g_pointers->m_send_event_ack(event_manager, source_player, target_player, event_index, event_handled_bitset);
					break;

				default:
					string msg = fmt::format(xorstr_("Purged unwanted protocol event: {} from: {}"), event_name, sender_name);

					LOG(WARNING) << msg;
					if (g_config.settings.notify_debug)
						g_notification_service->push_warning(xorstr_("Event Protocol"), msg);

					return;
				}
			}
		}

		switch ((RockstarEvent)event_id)
		{ //Begin switch case

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
						string msg = fmt::format(xorstr_("{} attempted to send TASK_VEHICLE_TEMP_ACTION crash."), sender_name);

						LOG(WARNING) << msg;
						g_notification_service->push_warning(xorstr_("Protection"), msg);

						return;
					}

					buffer->Seek(0);
				}
			}

			break;
		}

		case RockstarEvent::SCRIPTED_GAME_EVENT:
		{
			if (g_config.protection.kick.script_events)
			{
				const auto scripted_game_event = std::make_unique<CScriptedGameEvent>();

				buffer->ReadDword(&scripted_game_event->m_args_size, 32);

				if (scripted_game_event->m_args_size - 1 <= 0x1AF)
					buffer->ReadArray(&scripted_game_event->m_args, 8 * scripted_game_event->m_args_size);

				const auto args = scripted_game_event->m_args;
				const auto hash = static_cast<eRemoteEvent>(args[0]);

				/*if (g_config.settings.notify_debug)
					LOG(INFO) << fmt::format(xorstr_("Player: {} sent script event: {}"), source_player->get_name(), int(hash));*/

				if (hooks::scripted_game_event(scripted_game_event.get(), source_player))
					return;

				buffer->Seek(0);
			}

			break;
		}

		} //End switch case

		//I know having two switch cases is stupid, but it's just a placeholder for now.
		if (g_config.protection.misc.modder_detection)
		{
			switch ((RockstarEvent)event_id)
			{ //Begin switch case

			case RockstarEvent::REPORT_CASH_SPAWN_EVENT:
			{
				uint32_t money;

				buffer->Seek(64);
				buffer->ReadDword(&money, 32);
				buffer->Seek(0);

				if (money >= 2000)
				{
					string msg = fmt::format(xorstr_("{} was flagged as a modder for spawning cash"), sender_name);

					LOG(WARNING) << msg;
					g_notification_service->push_warning(xorstr_("Modder detection"), msg);
				}

				return;
			}

			case RockstarEvent::NETWORK_CHECK_CODE_CRCS_EVENT:
			case RockstarEvent::REPORT_MYSELF_EVENT:
			{
				string msg = fmt::format(xorstr_("{} was flagged as a modder for sending unwanted events"), sender_name);

				LOG(WARNING) << msg;
				g_notification_service->push_warning(xorstr_("Modder detection"), msg);

				return;
			}
			}

		} //End switch case

		if (g_config.protection.kick.game_events)
		{
			//todo: add game event protection
		}

		return g_hooking->m_received_event_hook.get_original<decltype(&received_event)>()(event_manager, source_player, target_player, event_id, event_index, event_handled_bitset, unk, buffer);
	}
}