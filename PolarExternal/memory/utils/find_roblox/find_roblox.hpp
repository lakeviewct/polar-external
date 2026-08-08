#pragma once

#include <cstdint>
#include <string>

#include <Windows.h>
#include <TlHelp32.h>

	// find_roblox.cpp
PROCESSENTRY32W FindRobloxProc();
DWORD FindRobloxPID();
uint64_t FindRobloxBase();