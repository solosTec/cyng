/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_VM_ASIO_DOMAIN_H
#define CYNG_VM_ASIO_DOMAIN_H

#include <boost/asio.hpp>

namespace cyng 
{
	class controller;
	void register_socket(boost::asio::ip::tcp::socket& s, controller& vm);
}

#endif	//	CYNG_VM_ASIO_DOMAIN_H




