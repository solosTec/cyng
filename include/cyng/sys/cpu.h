/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_SYS_CPU_H
#define CYNG_SYS_MEMORY_H

#include <cstdint>

namespace cyng {
	namespace sys
	{
		/**
		 * @brief CPU load
		 * 
		 * core number 0 addresses all CPUs.
		 *
		 * @return CPU load of specified CPU core
		 */
		double get_cpu_load(std::size_t core);

	}
}
#endif

