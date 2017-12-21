/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SYS_CPU_H
#define CYNG_SYS_CPU_H

#include <cstdint>

namespace cyng 
{
	namespace sys 
	{
		double get_total_cpu_load();
		std::uint64_t get_cpu_load_by_process();
	}
}

#endif
