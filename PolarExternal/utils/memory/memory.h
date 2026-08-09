// memory core

#pragma once
#include <Windows.h>

#include <iostream>
#include <memory>
#include <vector>
#include <TlHelp32.h>

#include <memoryapi.h>

namespace mem
{
	class memory
	{
	public:
		std::uint32_t find_process_id(const std::string& process_name);
		std::uint64_t find_module_address(const std::string& module_name);

		bool attach_to_process(const std::string& process_name);

		std::string read_string(std::uint64_t address);

		std::uint32_t get_process_id();
		std::uint64_t get_module_address();
		HANDLE get_process_handle();

		template <typename T>
		T read(std::uint64_t address);

		template <typename T>
		void write(std::uint64_t address, T value);
	private:
		std::uint32_t process_id;
		std::uint64_t base_address;
		HANDLE process_handle;
	};
}

template <typename T>
T mem::memory::read(uint64_t address)
{
	T buffer;
	ReadProcessMemory(process_handle, reinterpret_cast<void*>(address), &buffer, sizeof(T), nullptr);
	return buffer;
}
template <typename T>
void mem::memory::write(uint64_t address, T value)
{
	ReadProcessMemory(process_handle, reinterpret_cast<void*>(address), &value, sizeof(T), nullptr);
}


inline std::unique_ptr<mem::memory> memory = std::make_unique<mem::memory>();