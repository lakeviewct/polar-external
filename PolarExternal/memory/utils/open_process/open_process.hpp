#pragma once

#include <Windows.h>

	// open_process.cpp
HANDLE OpenRobloxProcess();
HANDLE OpenRobloxProcessPID(DWORD roblox_pid);