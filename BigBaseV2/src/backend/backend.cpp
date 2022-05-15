#include "backend.hpp"
#include "common.hpp"
#include "script_mgr.hpp"
#include "fiber_pool.hpp"
#include "script_global.hpp"

namespace big
{
	void backend_main::run_tick()
	{
		QUEUE_JOB_BEGIN_CLAUSE(= )
		{
			backend_looped::update_pointers();
			backend_looped::hud_transition_state();

			//Temporary no idle kick spot
			*script_global(1644209).at(1149).as<int*>() = 0;
			*script_global(1644209).at(1165).as<int*>() = 0;

		} QUEUE_JOB_END_CLAUSE
	}

	void backend_main::script_func()
	{
		while (true)
		{
			TRY_CLAUSE
			{
				backend_main::run_tick();
			}
			EXCEPT_CLAUSE

			script::get_current()->yield();
		}
	}
}
