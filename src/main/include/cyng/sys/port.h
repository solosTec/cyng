/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SYS_PORT_H
#define CYNG_SYS_PORT_H

#include <string>
#include <vector>

namespace cyng 
{
	namespace sys 
	{
		/**
		 * list of available serial/COM ports
		 */
		std::vector<std::string>	get_ports();
	}
}

#endif
