#include "find_roblox.hpp"

PROCESSENTRY32W FindRobloxProc()
{
	// -----------

	HANDLE snapshot;

	PROCESSENTRY32W current_process{};
	bool search;
	
	// -----------

	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (snapshot == INVALID_HANDLE_VALUE)
		{
			return {};
		}

	current_process.dwSize = sizeof(current_process);
	search = Process32FirstW(snapshot, &current_process);

	// -----------

	while (search)
	{
		// compare name of current process with L"RobloxPlayerBeta.exe"
		if (_wcsicmp(current_process.szExeFile, L"RobloxPlayerBeta.exe") == false)
		{
			CloseHandle(snapshot);
			return current_process;
		}

		search = Process32NextW(snapshot, &current_process);
	}

	// -----------

	CloseHandle(snapshot);
	return current_process;
}

DWORD FindRobloxPID()
{
	// -----------

	HANDLE snapshot;

	PROCESSENTRY32W current_process{};
	bool search;

	// -----------

	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	current_process.dwSize = sizeof(current_process);
	search = Process32FirstW(snapshot, &current_process);

	// -----------

	while (search)
	{
		// compare name of current process with L"RobloxPlayerBeta.exe"
		if (_wcsicmp(current_process.szExeFile, L"RobloxPlayerBeta.exe") == false)
		{
			CloseHandle(snapshot);
			return current_process.th32ProcessID;
		}

		search = Process32NextW(snapshot, &current_process);
	}

	// -----------

	CloseHandle(snapshot);
	return current_process.th32ProcessID;
}

uint64_t FindRoblox_Base()
{
	// to be done
}