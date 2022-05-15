#pragma once
#include "imgui.h"
#include "natives.hpp"
#include "pointers.hpp"
#include "script.hpp"
#include "services/player_service.hpp"


namespace big
{
	class view
	{
	public:
		static void notifications();
		static void root();

		static void always()
		{
			notifications();
		}
	};
}