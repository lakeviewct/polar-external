#include <iostream>

#include "memory/read/memory.hpp"

int main()
{
	mem::init();

	std::cout << "Roblox BASE: " << std::hex << mem::base_address << std::endl;

	return 0;
}