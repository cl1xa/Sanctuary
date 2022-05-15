#include "common.hpp"
#include "function_types.hpp"
#include "logger.hpp"
#include "gta/array.hpp"
#include "gta/player.hpp"
#include "gta/script_thread.hpp"
#include "gui.hpp"
#include "hooking.hpp"
#include "memory/module.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "renderer.hpp"
#include "script_mgr.hpp"

#include <MinHook.h>

namespace big
{
	hooking::hooking() :
		// Swapchain
		m_swapchain_hook(*g_pointers->m_swapchain, hooks::swapchain_num_funcs),

		m_set_cursor_pos_hook("SetCursorPos", memory::module("user32.dll").get_export("SetCursorPos").as<void*>(), &hooks::set_cursor_pos),
		m_run_script_threads_hook("Script Hook", g_pointers->m_run_script_threads, &hooks::run_script_threads),
		m_convert_thread_to_fiber_hook("ConvertThreadToFiber", memory::module("kernel32.dll").get_export("ConvertThreadToFiber").as<void*>(), &hooks::convert_thread_to_fiber),

		m_gta_thread_start_hook("GTA Thead Start", g_pointers->m_gta_thread_start, &hooks::gta_thread_start),
		m_gta_thread_tick_hook("GTA Thread Tick", g_pointers->m_gta_thread_tick, &hooks::gta_thread_tick),
		m_gta_thread_kill_hook("GTA Thread Kill", g_pointers->m_gta_thread_kill, &hooks::gta_thread_kill),

		m_network_player_mgr_shutdown_hook("Network Player Mgr Shutdown", g_pointers->m_network_player_mgr_shutdown, &hooks::network_player_mgr_shutdown),

		m_network_group_override("Network Group Override", g_pointers->m_network_group_override, &hooks::network_group_override),

		m_net_array_handler_hook("Net Array Handler", g_pointers->m_net_array_handler, &hooks::net_array_handler),

		m_increment_stat_hook("Increment Stat Event", g_pointers->m_increment_stat_event, &hooks::increment_stat_event),

		m_is_dlc_present_hook("Is DLC Present", g_pointers->m_is_dlc_present, &hooks::is_dlc_present),

		m_received_event_hook("Received Event", g_pointers->m_received_event, &hooks::received_event),

		m_send_net_info_to_lobby("Send NET Info to Lobby", g_pointers->m_send_net_info_to_lobby, &hooks::send_net_info_to_lobby),

		m_player_has_joined_hook("Player Has Joined", g_pointers->m_player_has_joined, &hooks::player_join),
		
		m_player_has_left_hook("Player Has Left", g_pointers->m_player_has_left, &hooks::player_leave)
	{
		m_swapchain_hook.hook(hooks::swapchain_present_index, &hooks::swapchain_present);
		m_swapchain_hook.hook(hooks::swapchain_resizebuffers_index, &hooks::swapchain_resizebuffers);

		g_hooking = this;
	}

	hooking::~hooking()
	{
		if (m_enabled)
			disable();

		g_hooking = nullptr;
	}

	void hooking::enable()
	{
		m_swapchain_hook.enable();
		m_og_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(g_pointers->m_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&hooks::wndproc)));
		m_set_cursor_pos_hook.enable();

		m_run_script_threads_hook.enable();
		m_convert_thread_to_fiber_hook.enable();

		m_gta_thread_start_hook.enable();
		m_gta_thread_kill_hook.enable();
		m_gta_thread_tick_hook.enable();

		m_network_player_mgr_shutdown_hook.enable();

		m_net_array_handler_hook.enable();

		m_player_has_joined_hook.enable();
		m_player_has_left_hook.enable();

		m_increment_stat_hook.enable();

		m_received_event_hook.enable();

		m_send_net_info_to_lobby.enable();

		m_enabled = true;

		//g_gui.m_opened = true;
	}

	void hooking::disable()
	{
		m_enabled = false;

		m_send_net_info_to_lobby.disable();

		m_received_event_hook.disable();

		m_increment_stat_hook.disable();

		m_player_has_joined_hook.disable();
		m_player_has_left_hook.disable();

		m_net_array_handler_hook.disable();

		m_network_player_mgr_shutdown_hook.disable();

		m_gta_thread_tick_hook.disable();
		m_gta_thread_kill_hook.disable();
		m_gta_thread_start_hook.disable();

		m_convert_thread_to_fiber_hook.disable();
		m_run_script_threads_hook.disable();

		m_set_cursor_pos_hook.disable();
		SetWindowLongPtrW(g_pointers->m_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_og_wndproc));
		m_swapchain_hook.disable();
	}

	minhook_keepalive::minhook_keepalive()
	{
		MH_Initialize();
	}

	minhook_keepalive::~minhook_keepalive()
	{
		MH_Uninitialize();
	}

	bool hooks::run_script_threads(std::uint32_t ops_to_execute)
	{
		TRY_CLAUSE
		{
			if (g_running)
			{
				g_script_mgr.tick();
			}

			return g_hooking->m_run_script_threads_hook.get_original<functions::run_script_threads_t>()(ops_to_execute);
		} EXCEPT_CLAUSE
		return false;
	}

	void *hooks::convert_thread_to_fiber(void *param)
	{
		TRY_CLAUSE
		{
			if (IsThreadAFiber())
			{
				return GetCurrentFiber();
			}

			return g_hooking->m_convert_thread_to_fiber_hook.get_original<decltype(&convert_thread_to_fiber)>()(param);
		} EXCEPT_CLAUSE
		return nullptr;
	}

	HRESULT hooks::swapchain_present(IDXGISwapChain *this_, UINT sync_interval, UINT flags)
	{
		TRY_CLAUSE
		{
			if (g_running)
			{
				g_renderer->on_present();
			}

			return g_hooking->m_swapchain_hook.get_original<decltype(&swapchain_present)>(swapchain_present_index)(this_, sync_interval, flags);
		} EXCEPT_CLAUSE
		return NULL;
	}

	HRESULT hooks::swapchain_resizebuffers(IDXGISwapChain * this_, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swapchain_flags)
	{
		TRY_CLAUSE
		{
			if (g_running)
			{
				g_renderer->pre_reset();

				auto result = g_hooking->m_swapchain_hook.get_original<decltype(&swapchain_resizebuffers)>(swapchain_resizebuffers_index)
					(this_, buffer_count, width, height, new_format, swapchain_flags);

				if (SUCCEEDED(result))
				{
					g_renderer->post_reset();
				}

				return result;
			}

			return g_hooking->m_swapchain_hook.get_original<decltype(&swapchain_resizebuffers)>(swapchain_resizebuffers_index)
				(this_, buffer_count, width, height, new_format, swapchain_flags);
		} EXCEPT_CLAUSE
		return NULL;
	}

	LRESULT hooks::wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		TRY_CLAUSE
		{
			if (g_running)
			{
				g_renderer->wndproc(hwnd, msg, wparam, lparam);
			}

			return CallWindowProcW(g_hooking->m_og_wndproc, hwnd, msg, wparam, lparam);
		} EXCEPT_CLAUSE
		return NULL;
	}

	BOOL hooks::set_cursor_pos(int x, int y)
	{
		TRY_CLAUSE
		{
			if (g_gui.m_opened)
				return true;

			return g_hooking->m_set_cursor_pos_hook.get_original<decltype(&set_cursor_pos)>()(x, y);
		} EXCEPT_CLAUSE
		return FALSE;
	}
}
