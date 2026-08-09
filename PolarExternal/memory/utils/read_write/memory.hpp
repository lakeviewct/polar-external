#pragma once

#include <iostream>
#include <memoryapi.h>

namespace mem
{
	void init();

	std::uint64_t base_address;
	DWORD pid;

	template <typename read_type>
	read_type ReadRobloxMemory(std::uint64_t address, HANDLE roblox_handle)
	{
		read_type buffer{};
		ReadProcessMemory(roblox_handle, reinterpret_cast<void*>(address), &buffer, sizeof(T), nullptr);

		return buffer;
	}

	template <typename write_type>
	bool WriteRobloxMemory(write_type value, uint64_t address, HANDLE roblox_handle)
	{
		 return WriteProcessMemory(roblox_handle, reinterpret_cast<LPVOID>(address), &value, sizeof(write_type), nullptr);
	}
}