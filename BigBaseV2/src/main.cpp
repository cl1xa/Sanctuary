﻿#include "common.hpp"
#include "fiber_pool.hpp"
#include "gui.hpp"
#include "logger.hpp"
#include "hooking.hpp"
#include "pointers.hpp"
#include "renderer.hpp"
#include "script_mgr.hpp"
#include "thread_pool.hpp"

#include "native_hooks/native_hooks.hpp"
#include "services/player_service.hpp"
#include "services/notification_service.hpp"

#include "backend/backend.hpp"

BOOL APIENTRY DllMain(HMODULE hmod, DWORD reason, PVOID)
{
	using namespace big;
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hmod);

		g_hmodule = hmod;

		g_main_thread = CreateThread(nullptr, 0, [](PVOID) -> DWORD
		{
			while (!FindWindow(L"grcWindow", L"Grand Theft Auto V"))
				std::this_thread::sleep_for(1s);

			std::filesystem::path base_dir = std::getenv("appdata");
			base_dir /= "Sanctuary";

			auto file_manager_instance = std::make_unique<file_manager>(base_dir);

			auto logger_instance = std::make_unique<logger>(
				"Sanctuary",
				file_manager_instance->get_project_file("./cout.log")
			);

			try
			{
				LOG(INFO) << "Sanctuary Initializing";
				auto pointers_instance = std::make_unique<pointers>();
				LOG(INFO) << "Pointers initialized.";

				auto renderer_instance = std::make_unique<renderer>();
				LOG(INFO) << "Renderer initialized.";

				auto fiber_pool_instance = std::make_unique<fiber_pool>(10);
				LOG(INFO) << "Fiber pool initialized.";

				auto hooking_instance = std::make_unique<hooking>();
				LOG(INFO) << "Hooking initialized.";

				g_config.load("default.sanctuary");

				LOG(INFO) << "Settings Loaded.";

				auto thread_pool_instance = std::make_unique<thread_pool>();
				LOG(INFO) << "Thread pool initialized.";

				auto notification_service_instance = std::make_unique<notification_service>();
				auto player_service_instance = std::make_unique<player_service>();
				LOG(INFO) << "Registered service instances...";

				g_script_mgr.add_script(std::make_unique<script>(&backend_main::script_func));
				g_script_mgr.add_script(std::make_unique<script>(&gui::script_func));

				LOG(INFO) << "Scripts registered.";

				auto native_hooks_instance = std::make_unique<native_hooks>();
				LOG(INFO) << "Dynamic native hooker initialized.";

				g_hooking->enable();
				LOG(INFO) << "Hooking enabled.";

				g_running = true;

				while (g_running)
				{
					g_config.save("default.sanctuary");

					std::this_thread::sleep_for(500ms);
				}

				g_hooking->disable();
				LOG(INFO) << "Hooking disabled.";

				native_hooks_instance.reset();
				LOG(INFO) << "Dynamic native hooker uninitialized.";

				g_script_mgr.remove_all_scripts();
				LOG(INFO) << "Scripts unregistered.";

				player_service_instance.reset();
				LOG(INFO) << "Player Service reset.";
				notification_service_instance.reset();
				LOG(INFO) << "Notification Service reset.";

				LOG(INFO) << "All services have been reset.";

				// Make sure that all threads created don't have any blocking loops
				// otherwise make sure that they have stopped executing
				thread_pool_instance->destroy();
				LOG(INFO) << "Destroyed thread pool.";

				thread_pool_instance.reset();
				LOG(INFO) << "Thread pool uninitialized.";

				hooking_instance.reset();
				LOG(INFO) << "Hooking uninitialized.";

				fiber_pool_instance.reset();
				LOG(INFO) << "Fiber pool uninitialized.";

				renderer_instance.reset();
				LOG(INFO) << "Renderer uninitialized.";

				pointers_instance.reset();
				LOG(INFO) << "Pointers uninitialized.";
			}
			catch (std::exception const &ex)
			{
				LOG(WARNING) << ex.what();
			}

			LOG(INFO) << "Farewell!";

			logger_instance->destroy();
			logger_instance.reset();
			file_manager_instance.reset();

			CloseHandle(g_main_thread);
			FreeLibraryAndExitThread(g_hmodule, 0);

		}, nullptr, 0, &g_main_thread_id);
	}

	return true;
}
