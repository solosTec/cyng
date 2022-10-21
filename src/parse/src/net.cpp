/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#include <cyng/parse/net.h>
#include <cyng/parse/buffer.h>

#include <iostream>

#include <boost/assert.hpp>
#include <boost/asio.hpp>

namespace cyng {

	boost::asio::ip::address to_ipv6(std::string inp) {
		boost::asio::ip::address_v6::bytes_type bytes;
		//boost::asio::detail::array<unsigned char, 16>
		BOOST_ASSERT_MSG(inp.size() == 32, "wrong input size");
		auto const addr = cyng::hex_to_buffer(inp);
		if (addr.size() == 16) {
			auto pos = addr.begin();
			for (auto& e : bytes) {
				e = *pos++;
			}
			return boost::asio::ip::make_address_v6(bytes);
		}
		return boost::asio::ip::address_v6();

	}


#if defined(BOOST_OS_LINUX_AVAILABLE)
	boost::asio::ip::address to_ipv6(std::string hex_str, std::string device) {

		boost::system::error_code ec;
		//	fe80000000000000f45607fffe46e821
		//	fe80::f456:7ff:fe46:e821
		//boost::asio::detail::array<unsigned char, 16>
		BOOST_ASSERT(!device.empty());
		auto const tmp = to_ipv6(hex_str, 0).to_string();
		auto const str = tmp + "%" + device;
		// std::cerr << std::endl << "***\t" << str << std::endl;
		auto const addr = boost::asio::ip::make_address_v6(str, ec);
		return (!ec)
			? addr
			: boost::asio::ip::address_v6()
			;
	}
#endif

}

