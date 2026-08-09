#include "find_datamodel.hpp"
#include "../find_roblox/find_roblox.hpp"		// FindRobloxPID(), FindRobloxBase()
#include "../open_process/open_process.hpp"		// OpenRobloxProcessPID()

uint64_t FindFakeDatamodel()
{
	// ----------

	HANDLE handle_to_roblox;

	uint64_t fake_datamodel_pointer;	// stores the address of the fake dm pointer
	uint64_t fake_datamodel_address{};

	bool read_success{};

	// ----------

	handle_to_roblox = OpenRobloxProcessPID(FindRobloxPID());

	fake_datamodel_pointer = FindRobloxBase() + Offsets::FakeDataModel::Pointer;

	// ----------

	if (handle_to_roblox != nullptr)
	{
		read_success = ReadProcessMemory(handle_to_roblox, reinterpret_cast<LPCVOID>(fake_datamodel_pointer), &fake_datamodel_address, sizeof(uint64_t), nullptr);
	}

	if (!read_success)
	{
		if (handle_to_roblox != nullptr)
		{
			CloseHandle(handle_to_roblox);
		}

		return 0;
	}

	// ----------

	CloseHandle(handle_to_roblox);
	return fake_datamodel_address;
}

uint64_t FindDatamodel()
{
	// ----------

	HANDLE handle_to_roblox;

	uint64_t fake_datamodel_address;
	uint64_t real_datamodel_address{};

	bool read_success{};

	// ----------

	handle_to_roblox = OpenRobloxProcessPID(FindRobloxPID());

	fake_datamodel_address = FindFakeDatamodel();

	// ----------

	if (handle_to_roblox != nullptr && fake_datamodel_address != 0)
	{
		read_success = ReadProcessMemory(handle_to_roblox, reinterpret_cast<LPCVOID>(fake_datamodel_address + Offsets::FakeDataModel::RealDataModel), &real_datamodel_address, sizeof(uint64_t), nullptr);
	}

	if (!read_success)
	{
		if (handle_to_roblox != nullptr)
		{
			CloseHandle(handle_to_roblox);
		}

		return 0;
	}

	// ----------

	CloseHandle(handle_to_roblox);
	return real_datamodel_address;
}