#include "open_process.hpp"
#include "../find_roblox/find_roblox.hpp"

HANDLE OpenRobloxProcess()
{
	HANDLE roblox_handle;
	DWORD roblox_pid = FindRobloxPID();

	roblox_handle = OpenProcess(PROCESS_ALL_ACCESS, false, roblox_pid);

	return roblox_handle;
}

HANDLE OpenRobloxProcessPID(DWORD roblox_pid)
{
	HANDLE roblox_handle = OpenProcess(PROCESS_ALL_ACCESS, false, roblox_pid);

	return roblox_handle;
}