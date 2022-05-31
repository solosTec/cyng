/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_SYS_DNS_H
#define CYNG_SYS_DNS_H

#include <vector>
#include <boost/asio.hpp>

namespace cyng 
{
	namespace sys 
	{
		/**
		 * get list of DNS servers
		 */
		std::vector<boost::asio::ip::address> get_dns_servers();
	}
}

#endif
