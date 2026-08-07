#include "find_roblox.hpp"

PROCESSENTRY32W FindRobloxProc()
{
	// -----------

	HANDLE snapshot;

	PROCESSENTRY32W current_process{};
	
	// -----------

	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE)
	{
		return {};
	}

	current_process.dwSize = sizeof(current_process);

	// -----------

	if (Process32FirstW(snapshot, &current_process));
	{
		do
		{
			// compare name of current process with L"RobloxPlayerBeta.exe"
			if (_wcsicmp(current_process.szExeFile, L"RobloxPlayerBeta.exe") == false)
			{
				CloseHandle(snapshot);
				return current_process;
			}
		} while (Process32NextW(snapshot, &current_process));
	}

	// -----------

	CloseHandle(snapshot);
	return current_process;
}

DWORD FindRobloxPID()
{
	return FindRobloxProc().th32ProcessID;
}

uint64_t FindRobloxBase()
{
	// ----------

	PROCESSENTRY32W roblox_process;
	DWORD roblox_pid;
	uint64_t roblox_base_address{};

	HANDLE module_snapshot;
	MODULEENTRY32W module_entry{};

	bool search;

	// ----------

	roblox_process = FindRobloxProc();
	roblox_pid = roblox_process.th32ProcessID;

	module_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, roblox_pid);
	module_entry.dwSize = sizeof(module_entry);

	// ----------

	search = Module32FirstW(module_snapshot, &module_entry);

	while (search)
	{
		if (wcsicmp(module_entry.szModule, L"RobloxPlayerBeta.exe") == false)
		{
			roblox_base_address = reinterpret_cast<uint64_t>(module_entry.modBaseAddr);
			CloseHandle(module_snapshot);
			return roblox_base_address;
		}

		search = Module32NextW(module_snapshot, &module_entry);
	}

	// ----------

	CloseHandle(module_snapshot);
	return roblox_base_address;
}