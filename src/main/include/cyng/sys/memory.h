/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SYS_MEMORY_H
#define CYNG_SYS_MEMORY_H

#include <cstdint>

namespace cyng 
{
	namespace sys 
	{
		std::uint64_t get_total_virtual_memory();
		std::uint64_t get_used_virtual_memory();
		std::uint8_t get_used_virtual_memory_in_percent();
		std::uint64_t get_used_virtual_memory_by_process();

		std::uint64_t get_total_physical_memory();
		std::uint64_t get_used_physical_memory();
		std::uint8_t get_used_physical_memory_in_percent();
		std::uint64_t get_used_physical_memory_by_process();
		std::uint64_t get_used_virtual_memory_by_process();
	}
}

#endif
