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
			uint16_t _event_id = event_id;
			const auto event_name = *(char**)((DWORD64)event_manager + 8i64 * _event_id + 243376);

			bool null = (event_name == nullptr) || (source_player == nullptr);
			bool catalog = ((RockstarEvent)event_id > RockstarEvent::NETWORK_CHECK_CATALOG_CRC) || (event_id > 91u);
			bool valid = (source_player->m_player_id < 0) || (source_player->m_player_id >= 32);

			if (null || catalog || valid)
			{
				if (g_config.settings.notify_debug)
					g_notification_service->push_warning(xorstr_("Event protocol"), fmt::format(xorstr_("{} sent unwanted event protocol data"), sender_name));

				g_pointers->m_send_event_ack(event_manager, source_player, target_player, event_index, event_handled_bitset); //Send event back to them

				return; //Block event
			}
		}

		switch ((RockstarEvent)event_id)
		{ //Begin switch case

		//Go away. Forced on upon inject.
		case RockstarEvent::NETWORK_TRAIN_REPORT_EVENT:
		case RockstarEvent::NETWORK_TRAIN_REQUEST_EVENT:
			return;

		//Modder detection
		if (g_config.protection.misc.modder_detection)
		{
			case RockstarEvent::REPORT_CASH_SPAWN_EVENT:
			case RockstarEvent::NETWORK_CHECK_CODE_CRCS_EVENT:
			case RockstarEvent::REPORT_MYSELF_EVENT:
			{
				g_notification_service->push_warning(xorstr_("Modder detection"), fmt::format(xorstr_("{} was flagged as a modder for sending unwanted events"), sender_name));

				return; //Block event
			}
		}

		if (g_config.protection.crash.task_vehicle_temp_action)
		{
		case RockstarEvent::SCRIPT_ENTITY_STATE_CHANGE_EVENT:
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

				if (action >= 15 && action <= 18)
				{
					g_notification_service->push_warning(xorstr_("Protection"), fmt::format(xorstr_("{} attempted to send TASK_VEHICLE_TEMP_ACTION crash"), sender_name));

					g_pointers->m_send_event_ack(event_manager, source_player, target_player, event_index, event_handled_bitset);//Send event back to them

					return;	//Block event
				}

				buffer->Seek(0);
			}
		}
		break; //Pass
		}

		if (g_config.protection.events.script)
		{
		case RockstarEvent::SCRIPTED_GAME_EVENT:
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
					g_pointers->m_send_event_ack(event_manager, source_player, target_player, event_index, event_handled_bitset); //Send event back to them

					return;	//Block event
				}

				buffer->Seek(0);
			}
			break; //Pass
		}

		if (g_config.protection.events.control)
		{
		case RockstarEvent::GIVE_CONTROL_EVENT:
		case RockstarEvent::VEHICLE_COMPONENT_CONTROL_EVENT:
		case RockstarEvent::MODIFY_VEHICLE_LOCK_WORD_STATE_DATA:
		case RockstarEvent::ACTIVATE_VEHICLE_SPECIAL_ABILITY_EVENT:

			g_notification_service->push_warning(xorstr_("Protection"), fmt::format(xorstr_("{} sent unwanted control event"), sender_name));

			return; //Block event
		}

		if (g_config.protection.events.action)
		{
		case RockstarEvent::BREAK_PROJECTILE_TARGET_LOCK_EVENT:
		case RockstarEvent::START_PROJECTILE_EVENT:
		case RockstarEvent::FIRE_EVENT:
		case RockstarEvent::EXPLOSION_EVENT:
		case RockstarEvent::WEAPON_DAMAGE_EVENT:
		case RockstarEvent::INCIDENT_ENTITY_EVENT:
		case RockstarEvent::BLOW_UP_VEHICLE_EVENT:
		case RockstarEvent::INFORM_SILENCED_GUNSHOT_EVENT:
		case RockstarEvent::UPDATE_PLAYER_SCARS_EVENT:

			g_notification_service->push_warning(xorstr_("Protection"), fmt::format(xorstr_("{} sent unwanted action event"), sender_name));

			return;	//Block event
		}

		if (g_config.protection.events.game)
		{
		case RockstarEvent::NETWORK_CLEAR_PED_TASKS_EVENT:
		case RockstarEvent::NETWORK_START_PED_ARREST_EVENT:
		case RockstarEvent::NETWORK_START_PED_UNCUFF_EVENT:
		case RockstarEvent::NETWORK_PTFX_EVENT:
		case RockstarEvent::GIVE_PED_SEQUENCE_TASK_EVENT:
		case RockstarEvent::GIVE_PED_SCRIPTED_TASK_EVENT:
		case RockstarEvent::GIVE_WEAPON_EVENT:
		case RockstarEvent::GIVE_PICKUP_REWARDS_EVENT:
		case RockstarEvent::REMOVE_WEAPON_EVENT:
		case RockstarEvent::REMOVE_ALL_WEAPONS_EVENT:
		case RockstarEvent::RAGDOLL_REQUEST_EVENT:
		case RockstarEvent::REQUEST_DETACHMENT_EVENT:
		case RockstarEvent::REQUEST_CONTROL_EVENT:
		case RockstarEvent::GAME_WEATHER_EVENT:
		case RockstarEvent::GAME_CLOCK_EVENT:
		case RockstarEvent::ALTER_WANTED_LEVEL_EVENT:

			g_notification_service->push_warning(xorstr_("Protection"), fmt::format(xorstr_("{} sent unwanted game event"), sender_name));

			return;	//Block event
		}

		default:
			break;

		} //End switch case

		return g_hooking->m_received_event_hook.get_original<decltype(&received_event)>()(event_manager, source_player, target_player, event_id, event_index, event_handled_bitset, unk, buffer);
	}
}