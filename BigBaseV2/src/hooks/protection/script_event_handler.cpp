#include "hooking.hpp"
#include "gta_util.hpp"

namespace big
{
	bool hooks::scripted_game_event(CScriptedGameEvent* scripted_game_event, CNetGamePlayer* player)
	{
		const auto args = scripted_game_event->m_args;
		const auto event = args[0];
		const auto player_name = player->get_name();

		//TODO:
			//https://pastebin.com/RZKQ3a1n
			//https://github.com/ccccloro/gta5-N0obMenu/blob/89a03d2f30bba4845f50e9bca69592fb9e19dac8/events.h#L141
			//https://github.com/cl52902/SaltyV2/blob/1ea5c7742408f2b34795da5baccc328c48f74a12/Salty/src/hooking.cpp#L475
			//https://github.com/cl52902/SaltyV2/blob/1ea5c7742408f2b34795da5baccc328c48f74a12/Salty/src/hooking.cpp#L407

		switch (event)
		{
			
		}

		return false;
	}
}