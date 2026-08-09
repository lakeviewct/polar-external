#include <iostream>

#include <chrono>
#include <thread>

#include "utils/memory/memory.h"
#include "utils/logging/log.h"
#include "game/game.h"

std::string process_name = "RobloxPlayerBeta.exe";
int pid;

// we use WinMain becuase logging::init() already opens and allocates a console for us 
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	logging::init();

	std::uint64_t ms_start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

	// init logic

	if (!memory->find_process_id(process_name.c_str()))
		logging::info("Waiting for roblox...");

	while (!(pid = memory->find_process_id((process_name.c_str()))))
	{
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}

	logging::ok("Found roblox with PID %d", pid); // pid cached so we dont have to call find_process_id again

	logging::info("loading...");

	if (!memory->attach_to_process(process_name.c_str()))
	{
		logging::fail("unable to attach to roblox\n");
		std::this_thread::sleep_for(std::chrono::seconds(2));
		return 1;
	}

	if (!memory->find_module_address(process_name.c_str()))
	{
		logging::fail("unable to find main module address\n");
		std::this_thread::sleep_for(std::chrono::seconds(2));
		return 1;
	}

	// fill the variables using the offsets 
	std::uint64_t fake_datamodel = memory->read<std::uint64_t>(memory->get_module_address() + Offsets::FakeDataModel::Pointer);
	game::datamodel = rbx::instance(memory->read<std::uint64_t>(fake_datamodel + Offsets::FakeDataModel::RealDataModel));
	game::visengine = rbx::visualengine(memory->read<std::uint64_t>(memory->get_module_address() + Offsets::VisualEngine::Pointer));
	game::players = rbx::instance(game::datamodel.find_first_child_by_class("Players"));

	// WINDOWSCLIENT is the window class (rtfm) and Roblox is the window name, you can get this too by looking at roblox in Spy++
	game::roblox_hwnd = FindWindowA("WINDOWSCLIENT", "Roblox");

	// as you see we dont need 40 lines to find the datamodel because we can just use memory->read<T>(address)

	// no idea what 0x%llx does ai filled it up if this was std::cout it would proby be std::hex
	logging::info("Found roblox module address at 0x%llx", memory->get_module_address());

	std::uint64_t ms_end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

	logging::ok("Finished loading at %d ms", ms_end - ms_start);

	std::this_thread::sleep_for(std::chrono::seconds(2));
	
	return 0;
}