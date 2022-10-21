/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_SYS_MEMORY_H
#define CYNG_SYS_MEMORY_H

#include <cstdint>

namespace cyng {
	namespace sys
	{
		/**
		 * @return available physical memory in system in bytes
		 */
		std::uint64_t get_total_ram();

		/**
		 * @return total ram - available ram
		 */
		std::uint64_t get_used_ram();

		/**
		 * @return available virtual memory in system in bytes
		 */
		std::uint64_t get_total_virtual_memory();

		/**
		 * @return available total virtual memory - available virtual memory
		 */
		std::uint64_t get_used_virtual_memory();

	}
}
#endif

