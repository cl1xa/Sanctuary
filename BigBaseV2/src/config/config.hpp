#pragma once
#include "gta/enums.hpp"
#include <imgui.h>

#ifndef GLOBALS_H
#define GLOBALS_H

namespace big
{
	class config
	{
	public:
		static bool save(std::string file);
		static bool load(std::string file);

		struct
		{
			struct
			{
				bool block_main_script = false;
				bool event_protocol_cleanup = false;
				bool script = false;

			}events;

			struct
			{
				bool net_array_handler = false;
				bool script_world_state = false;
				bool task_vehicle_temp_action = false;

			}crash;

			struct 
			{
				bool host_desync = false;
				bool clone_sync = false;

			}sync;

			struct
			{
				bool idle = false;
				bool chat_spam = false;
				bool terminate_loading_screens = false;
				bool rate_limit = false;
				bool modder_detection = false;
				bool optimize_loading = false;
				//bool block_reports = false;
				//bool block_stat_changes = false;

			}misc;

		}protection;

		struct
		{
			bool notify_players = false;
			bool notify_debug = false;
			bool notify_scripts = false;
			bool script_event_logger = false;

		}settings;
	};

	inline config g_config;
}
#endif // !GLOBALS_H