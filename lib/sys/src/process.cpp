/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#include <cyng/sys/process.h>
#include <boost/predef.h>
#include <boost/assert.hpp>
#include <iostream>
#include <boost/process/environment.hpp>

namespace cyng 
{
	namespace sys 
	{
		boost::process::pid_t get_process_id()
		{
			return boost::this_process::get_id();
		}
	}
}




