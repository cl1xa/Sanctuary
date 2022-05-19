#include "hooking.hpp"
#include "native_hooks/native_hooks.hpp"

namespace big
{
	rage::eThreadState hooks::gta_thread_kill(GtaThread* thread)
	{
		const auto result = g_hooking->m_gta_thread_kill_hook.get_original<decltype(&gta_thread_kill)>()(thread);

		string msg = fmt::format(xorstr_("Script Thread '{}' terminated."), thread->m_name);

		LOG(INFO) << msg;
		if (g_config.settings.notify_debug)
			g_notification_service->push(xorstr_("Script Thread Termination"), msg);

		g_native_hooks->do_cleanup_for_thread(thread);

		return result;
	}
}