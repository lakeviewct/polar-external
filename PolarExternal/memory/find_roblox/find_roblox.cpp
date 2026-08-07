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

uint64_t FindRoblox_Base()
{
	// to be done
}