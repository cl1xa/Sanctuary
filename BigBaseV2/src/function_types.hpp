#pragma once
#include "common.hpp"
#include "gta/fwddec.hpp"
#include "gta/player.hpp"
#include "gta/natives.hpp"
#include "gta/replay.hpp"

namespace big::functions
{
	#pragma region ENGINE
	using run_script_threads_t = bool(*)(std::uint32_t ops_to_execute);
	using get_native_handler_t = rage::scrNativeHandler(*)(rage::scrNativeRegistrationTable* registration_table, rage::scrNativeHash hash);
	using fix_vectors_t = void(*)(rage::scrNativeCallContext* call_ctx);

	using ptr_to_handle = Entity(*)(void* entity);
	#pragma endregion

	#pragma region EVENT
	using read_bitbuf_array = bool(rage::datBitBuffer* buffer, PVOID read, int bits, int);
	using read_bitbuf_dword = bool(rage::datBitBuffer* buffer, PVOID read, int bits);

	using send_event_ack = void(rage::netEventMgr* event_manager, CNetGamePlayer* source_player, CNetGamePlayer* target_player, int event_index, int event_handled_bitset);

	using trigger_script_event = void(*)(int event_group, int64_t* args, int arg_count, int player_bits);

	using increment_stat_event = bool(*)(uint64_t net_event_struct, int64_t sender, int64_t a3);
	#pragma endregion

	#pragma region SYNC
	using get_sync_type_info = const char* (uint16_t sync_type, char a2);
	using get_sync_tree_for_type = __int64(CNetworkObjectMgr* mgr, uint16_t sync_type);

	using get_net_object = rage::netObject* (__fastcall)(CNetworkObjectMgr* mgr, int16_t id, bool unk3);
	using get_net_object_for_player = rage::netObject* (__fastcall) (CNetworkObjectMgr*, int16_t, CNetGamePlayer*, bool);
	#pragma endregion
}
