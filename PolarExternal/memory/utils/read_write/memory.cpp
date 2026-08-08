#include "memory.hpp"

#include "../find_roblox/find_roblox.hpp"

void mem::init()
{
	pid = FindRobloxPID();
	base_address = FindRobloxBase();
}
