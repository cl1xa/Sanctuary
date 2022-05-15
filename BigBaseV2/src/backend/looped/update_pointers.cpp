#include "backend/backend.hpp"
#include "gta_util.hpp"

namespace big
{
	void backend_looped::update_pointers() { g_local_player = gta_util::get_local_ped(); }
}