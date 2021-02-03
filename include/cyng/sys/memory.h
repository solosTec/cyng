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
		 * @return available memory in system in bytes
		 */
		std::uint64_t get_total_physical_memory();
	}
}
#endif

