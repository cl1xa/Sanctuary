#include "hooking.hpp"
#include "native_hooks/native_hooks.hpp"

namespace big
{
	rage::eThreadState hooks::gta_thread_kill(GtaThread* thread)
	{
		rage::eThreadState result = g_hooking->m_gta_thread_kill_hook.get_original<decltype(&gta_thread_kill)>()(thread);

		LOG(INFO) << fmt::format(xorstr_("Script thread {} terminated"), thread->m_name);

		if (g_config.settings.notify_debug)
			g_notification_service->push(xorstr_("Script Thread Termination"), fmt::format(xorstr_("Script Thread '{}' terminated."), thread->m_name));

		g_native_hooks->do_cleanup_for_thread(thread);

		if (thread->m_script_hash == RAGE_JOAAT("freemode"))
			g_config.protection.freemode_terminated = !(result == rage::eThreadState::running);

		return result;
	}
}