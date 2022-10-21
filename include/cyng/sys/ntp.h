/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_SYS_NTP_H
#define CYNG_SYS_NTP_H

#include <string>
#include <vector>

namespace cyng 
{
	namespace sys 
	{
		/**
		 * list of configured NTP servers
		 */
		std::vector<std::string>	get_ntp_servers();
	}
}

#endif
