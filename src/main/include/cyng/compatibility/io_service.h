/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_COMPATIBILITY_IO_SERVICE_H
#define CYNG_COMPATIBILITY_IO_SERVICE_H

#include <boost/version.hpp>
#include <boost/asio.hpp>


namespace cyng 
{
	//
	//	note that the former io_context::wrap<H>() methods are replaced by
	//	boost::asio::bind_executor<typename Executor, typename T>()
	//

//#if (BOOST_VERSION >= 106600)
#if (BOOST_ASIO_VERSION < 101200)
	using io_service_t = boost::asio::io_service;
	using dispatcher_t = boost::asio::strand;

	inline boost::asio::ip::address make_address(std::string const& str)
	{
		//	dotted address notation
		return boost::asio::ip::address_v4::from_string(str);
	}
#else
	using io_service_t = boost::asio::io_context;
	using dispatcher_t = boost::asio::io_context::strand;

	inline boost::asio::ip::address make_address(std::string const& str)
	{
		//	dotted address notation
		return boost::asio::ip::make_address(str);
	}
#endif

}

#endif 	//	CYNG_COMPATIBILITY_IO_SERVICE_H





