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
			bool notify_players = false;
			bool notify_debug = false;
			bool optimize_loading = false;

		}settings;

		struct
		{
			bool freemode_terminated = false;

		}protection;
	};

	inline config g_config;
}
#endif // !GLOBALS_H