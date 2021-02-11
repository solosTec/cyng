/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_SYS_PORT_H
#define CYNG_SYS_PORT_H


#include <string>
#include <vector>

namespace cyng {
	namespace sys
	{
		/**
		 * @return list of available serial/COM ports
		 */
		std::vector<std::string> get_serial_ports();
	}
}
#endif

