#include "hooking.hpp"
#include "native_hooks/native_hooks.hpp"

namespace big
{
	rage::eThreadState hooks::gta_thread_kill(GtaThread* thread)
	{
		const auto result = g_hooking->m_gta_thread_kill_hook.get_original<decltype(&gta_thread_kill)>()(thread);

		if (g_config.settings.notify_scripts)
			g_notification_service->push(xorstr_("Script Threads"), fmt::format(xorstr_("Script Thread '{}' terminated"), thread->m_name));

		g_native_hooks->do_cleanup_for_thread(thread);

		return result;
	}
}