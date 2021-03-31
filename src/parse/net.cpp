/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#include <cyng/parse/net.h>
#include <cyng/parse/buffer.h>

#include <boost/assert.hpp>
#include <boost/asio.hpp>

namespace cyng {

	boost::asio::ip::address to_ipv6(std::string inp, unsigned long scope_id) {

		boost::asio::ip::address_v6::bytes_type bytes;
		//boost::asio::detail::array<unsigned char, 16>
		BOOST_ASSERT_MSG(inp.size() == 32, "wrong input size");
		auto const addr = cyng::hex_to_buffer(inp);
		if (addr.size() == 16) {
			auto pos = addr.begin();
			for (auto& e : bytes) {
				e = *pos++;
			}
			return boost::asio::ip::make_address_v6(bytes, scope_id);
		}
		//std::cout << boost::asio::ip::make_address_v6(t, 0) << std::endl;
		return boost::asio::ip::address();
	}

}

