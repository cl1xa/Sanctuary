
#include "backend/backend.hpp"
#include "gta_util.hpp"
#include "script_global.hpp"

namespace big
{
	void backend_looped::no_idle_kick()
	{
		if (g_config.protection.kick.idle)
		{
			*script_global(1644209).at(1149).as<int*>() = 0;
			*script_global(1644209).at(1165).as<int*>() = 0;
		}
	}
}
