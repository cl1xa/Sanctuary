#include "hooking.hpp"
#include "gta_util.hpp"

namespace big
{
	bool hooks::scripted_game_event(CScriptedGameEvent* scripted_game_event, CNetGamePlayer* player)
	{
		const auto args = scripted_game_event->m_args;
		const auto hash = static_cast<eRemoteEvent>(args[0]);
		const auto player_name = player->get_name();

		if ((int64_t)eRemoteEvent::RotateCam) //Rule this one out and check for specific circumstances due its constant false positives
		{
			if (CNetworkPlayerMgr* player_mgr = gta_util::get_network_player_mgr(); player_mgr != nullptr)
			{
				//We only want to log in this specific circumstance
				if (args[2] == player_mgr->m_local_net_player->m_player_id)
				{
					string msg = fmt::format(xorstr_("{} attempted to modify your camera"), player_name);

					LOG(WARNING) << msg;
					g_notification_service->push_warning(xorstr_("Protections"), msg);
				}
			}

			//Block it anyway regardless of the circumstances
			//Don't you just love the word circumstance?
			return true;
		}

		switch (hash)
		{
		case eRemoteEvent::Bounty:
		case eRemoteEvent::CeoBan:
		case eRemoteEvent::CeoKick:
		case eRemoteEvent::CeoMoney:
		case eRemoteEvent::ClearWantedLevel:
		case eRemoteEvent::Crash:
		case eRemoteEvent::FakeDeposit:
		case eRemoteEvent::ForceMission:
		case eRemoteEvent::GtaBanner:
		case eRemoteEvent::MCTeleport:
		case eRemoteEvent::NetworkBail:
		case eRemoteEvent::PersonalVehicleDestroyed:
		case eRemoteEvent::RemoteOffradar:
		case eRemoteEvent::SendToCutscene:
		case eRemoteEvent::SendToIsland:
		case eRemoteEvent::SoundSpam:
		case eRemoteEvent::Spectate:
		case eRemoteEvent::Teleport:
		case eRemoteEvent::TransactionError:
		case eRemoteEvent::VehicleKick:
			string msg = fmt::format(xorstr_("{} sent unwanted script event: {}"), player_name, hash);

			LOG(WARNING) << msg;
			g_notification_service->push_warning(xorstr_("Protections"), msg);

			return true;
		}

		return false;
	}
}