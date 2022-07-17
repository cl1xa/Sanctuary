#include "backend/backend.hpp"
#include "script_global.hpp"
#include "gta/structs.hpp"
#include "natives.hpp"

namespace big
{
	auto transition_state = script_global(1574988);
	eTransitionState last_state = eTransitionState::TRANSITION_STATE_EMPTY;

	void backend_looped::hud_transition_state()
	{
		const auto state = *transition_state.as<eTransitionState*>();

		//Terminate loading screen on freemode init
		if (state == eTransitionState::TRANSITION_STATE_WAIT_JOIN_FM_SESSION && DLC::GET_IS_LOADING_SCREEN_ACTIVE())
			SCRIPT::SHUTDOWN_LOADING_SCREEN();

		if (last_state == state || state == eTransitionState::TRANSITION_STATE_EMPTY || state > eTransitionState::TRANSITION_STATE_DLC_INTRO_BINK)
			return;

		if (HUD::BUSYSPINNER_IS_ON())
			HUD::BUSYSPINNER_OFF();

		//Fix for singleplayer transition
		if (state == eTransitionState::TRANSITION_STATE_TERMINATE_MAINTRANSITION)
			return;

		if ((int)state > 0 && (int)std::size(transition_states))
		{
			HUD::BEGIN_TEXT_COMMAND_BUSYSPINNER_ON("STRING");

			auto const spinner_text = fmt::format((xorstr_("Loading GTA Online... [{} | {}]")), transition_states[(int)state], state);

			HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(spinner_text.c_str());
			HUD::END_TEXT_COMMAND_BUSYSPINNER_ON(5);
		}

		last_state = state;
	}
}