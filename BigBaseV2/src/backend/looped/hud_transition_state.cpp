#include "backend/backend.hpp"
#include "script_global.hpp"
#include "gta/structs.hpp"

namespace big
{
	auto transition_state = script_global(1574988);
	eTransitionState last_state = eTransitionState::TRANSITION_STATE_MAX;

	void backend_looped::hud_transition_state()
	{
		if (g_config.protection.misc.terminate_loading_screens)
		{
			const auto state = *transition_state.as<eTransitionState*>();

			if (last_state == eTransitionState::TRANSITION_STATE_MAX)
				last_state = state;

			//When freemode script loads, remove the loading screen
			if (state == eTransitionState::TRANSITION_STATE_WAIT_JOIN_FM_SESSION && DLC::GET_IS_LOADING_SCREEN_ACTIVE())
			{
				string msg = fmt::format(xorstr_("Terminated loading screen: {}"), transition_states[(int)state]);

				LOG(G3LOG_DEBUG) << msg;
				g_notification_service->push(xorstr_("Transition State"), msg);

				SCRIPT::SHUTDOWN_LOADING_SCREEN();
			}

			if (last_state == state || state == eTransitionState::TRANSITION_STATE_EMPTY || state > eTransitionState::TRANSITION_STATE_DLC_INTRO_BINK)
				return;

			if (HUD::BUSYSPINNER_IS_ON())
				HUD::BUSYSPINNER_OFF();

			//Sometimes when going into singleplayer, missions, or a transition, this one will remain on the screen permanently
			if (state == eTransitionState::TRANSITION_STATE_TERMINATE_MAINTRANSITION)
				return;

			HUD::BEGIN_TEXT_COMMAND_BUSYSPINNER_ON("STRING");
			HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(fmt::format(xorstr_("Loading... [{}]"), transition_states[(int)state]).c_str());
			HUD::END_TEXT_COMMAND_BUSYSPINNER_ON(5);

			last_state = state;
		}
	}
}