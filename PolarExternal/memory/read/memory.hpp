#pragma once

#include <iostream>
#include <memoryapi.h>

namespace mem
{
	void init();

	std::uint64_t base_address;
	DWORD pid;

	template <typename T>
	T read(std::uint64_t address)
	{
		T buffer;
		ReadProcessMemory(process_handle, reinterpret_cast<void*>(address), &buffer, sizeof(T), nullptr);
		return buffer;
	}
}