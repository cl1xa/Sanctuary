#pragma once
#include "imgui.h"
#include "natives.hpp"
#include "pointers.hpp"
#include "script.hpp"
#include "services/player_service.hpp"


namespace big
{
	class view_main
	{
	public:
		static void notifications();
		static void root();

		static void always()
		{
			notifications();
		}
	};

	class view_sub
	{
	public:
		static void tab_events();
		static void tab_crash();
		static void tab_sync();
		static void tab_misc();
		static void tab_network();
		static void tab_settings();
	};
}