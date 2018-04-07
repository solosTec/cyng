/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SYS_PROCESS_H
#define CYNG_SYS_PROCESS_H

#include <cstdint>
#include <boost/process/child.hpp>

namespace cyng 
{
	namespace sys 
	{
		boost::process::pid_t get_process_id();
	}
}

#endif
