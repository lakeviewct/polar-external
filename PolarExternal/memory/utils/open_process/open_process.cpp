#include "open_process.hpp"
#include "../find_roblox/find_roblox.hpp"

// Slower since you internally call FindRobloxPID()
HANDLE OpenRobloxProcess()
{
	HANDLE roblox_handle;
	DWORD roblox_pid = FindRobloxPID();

	roblox_handle = OpenProcess(PROCESS_ALL_ACCESS, false, roblox_pid);

	return roblox_handle;
}

// Use this one if you already have the pid:
HANDLE OpenRobloxProcessPID(DWORD roblox_pid)
{
	HANDLE roblox_handle = OpenProcess(PROCESS_ALL_ACCESS, false, roblox_pid);

	return roblox_handle;
}