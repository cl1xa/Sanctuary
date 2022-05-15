#include "hooking.hpp"
#include "gta_util.hpp"

namespace big
{
	bool hooks::scripted_game_event(CScriptedGameEvent* scripted_game_event, CNetGamePlayer* player)
	{
		const auto args = scripted_game_event->m_args;

		const auto hash = static_cast<eRemoteEvent>(args[0]);
		const auto player_name = player->get_name();

		//TODO: add script event protection

		/*
			LOG(INFO) << "== Begin of Script Event ==";
			LOG(INFO) << "Player: " << player->get_name();
			LOG(INFO) << "Hash/Arg #0: " << (int)hash;

			for (std::size_t i = 1; i < sizeof(args); i++)
				LOG(INFO) << "Arg #" << i << ": " << args[i];

			LOG(INFO) << "== End of Script Event ==";
		*/

		return false;
	}
}