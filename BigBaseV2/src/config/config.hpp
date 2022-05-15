#pragma once
#include "gta/enums.hpp"
#include <imgui.h>

#ifndef GLOBALS_H
#define GLOBALS_H

using namespace std;

namespace big
{
	class config
	{
	public:
		static bool save(string file);
		static bool load(string file);

		struct
		{
			struct
			{
				bool script_events = false;
				bool game_events = false;

			}kick;

			struct
			{
				bool net_array_handler = false;
				bool script_world_state = false;
				bool task_vehicle_temp_action = false;

			}crash;

			struct
			{
				bool terminate_loading_screens = false;
				bool rate_limit = false;
				bool modder_detection = false;
				bool event_protocol_cleanup = false;
				bool optimize_loading = false;
				//bool block_reports = false;

			}misc;

			bool freemode_terminated = false;

		}protection;

		struct
		{
			bool notify_players = false;
			bool notify_debug = false;

		}settings;
	};

	inline config g_config;
}
#endif // !GLOBALS_H