/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_PARSER_EP_H
#define CYNG_PARSER_EP_H

#include <cyng/object.h>

#include <boost/config/warning_disable.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace cyng	
{
	
	/**
	 * Converts a string of hex values into a buffer.
	 */
	std::pair<boost::asio::ip::tcp::endpoint, bool > parse_tcp_ep(std::string const& inp);
	//object parse_ep_obj(std::string const& inp);


} 

#endif	

