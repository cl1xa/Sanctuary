#include "hooking.hpp"
#include "gta_util.hpp"

namespace big
{
	const int64_t blocked_script_events[] =
	{
		(int64_t)eRemoteEvent::Bounty,
		(int64_t)eRemoteEvent::CeoBan,
		(int64_t)eRemoteEvent::CeoKick,
		(int64_t)eRemoteEvent::CeoMoney,
		(int64_t)eRemoteEvent::ClearWantedLevel,
		(int64_t)eRemoteEvent::Crash,
		(int64_t)eRemoteEvent::FakeDeposit,
		(int64_t)eRemoteEvent::ForceMission,
		(int64_t)eRemoteEvent::GtaBanner,
		(int64_t)eRemoteEvent::MCTeleport,
		(int64_t)eRemoteEvent::NetworkBail,
		(int64_t)eRemoteEvent::PersonalVehicleDestroyed,
		(int64_t)eRemoteEvent::RemoteOffradar,
		(int64_t)eRemoteEvent::SendToCutscene,
		(int64_t)eRemoteEvent::SendToIsland,
		(int64_t)eRemoteEvent::SoundSpam,
		(int64_t)eRemoteEvent::Spectate,
		(int64_t)eRemoteEvent::Teleport,
		(int64_t)eRemoteEvent::TransactionError,
		(int64_t)eRemoteEvent::VehicleKick
	};

	bool hooks::scripted_game_event(CScriptedGameEvent* scripted_game_event, CNetGamePlayer* player)
	{
		const auto args = scripted_game_event->m_args;
		const auto hash = static_cast<eRemoteEvent>(args[0]);
		const auto player_name = player->get_name();
		const char* event_name = scripted_game_event->get_name();

		if ((int64_t)eRemoteEvent::RotateCam)
		{
			if (CNetworkPlayerMgr* player_mgr = gta_util::get_network_player_mgr(); player_mgr != nullptr)
			{
				if (args[2] == player_mgr->m_local_net_player->m_player_id)
				{
					LOG(WARNING) << fmt::format(xorstr_("{} attempted to modify your camera"), player_name);

					g_notification_service->push_warning(xorstr_("Protections"), fmt::format(xorstr_("{} attempted to modify your camera"), player_name));
				}
			}

			return true;
		}

		for (int a = 0; a < sizeof(blocked_script_events) / sizeof(blocked_script_events[0]); a++)
		{
			if (blocked_script_events[a] == (int64_t)hash)
			{
				LOG(WARNING) << fmt::format(xorstr_("{} sent unwanted script event: {} | {}"), player_name, event_name, hash);

				g_notification_service->push_warning(xorstr_("Protections"), fmt::format(xorstr_("{} sent unwanted script event: "), player_name, event_name, hash));

				return true;
			}
		}

		return false;
	}
}