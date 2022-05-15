#pragma once
#include "common.hpp"

namespace big
{
	class backend_main
	{
	public:
		static void run_tick();
		static void script_func();
	};

	class backend_looped
	{
	public:
		static void update_pointers();
		static void hud_transition_state();
	};
}
