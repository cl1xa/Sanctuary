#include "backend.hpp"
#include "common.hpp"
#include "script_mgr.hpp"
#include "fiber_pool.hpp"
#include "script_global.hpp"

namespace big
{
	void backend_main::run_tick()
	{
		QUEUE_JOB_BEGIN_CLAUSE()
		{
			backend_looped::update_pointers();
			backend_looped::hud_transition_state();
			backend_looped::no_idle_kick();

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
