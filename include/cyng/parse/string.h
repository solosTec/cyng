/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_PARSE_STRING_H
#define CYNG_PARSE_STRING_H

#include <cyng/obj/intrinsics/aes_key.hpp>
#include <cyng/obj/buffer_cast.hpp>
#include <cyng/obj/util.hpp>

#include <string>
#include <vector>
#include <chrono>

#include <boost/utility/string_view.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/asio/ip/address.hpp>

namespace cyng {
	std::vector<std::string> split(std::string str, std::string delims);
	std::vector<boost::string_view> split(boost::string_view str, boost::string_view delims);

	/**
	 * convert hex string to UUID
	 */
	boost::uuids::uuid to_uuid(std::string const&);

	/**
	 * 2021-03-31T19:36:45+0100	- ISO-8601
	 */
	std::chrono::system_clock::time_point to_tp_iso8601(std::string const&);

	/**
	 * "%Y-%m-%d %H:%M:%S" - SQLite datetime()
	 */
	std::chrono::system_clock::time_point to_tp_datetime(std::string const&);

	boost::asio::ip::address to_ip_address(std::string const&);

	template <std::size_t N>
	auto to_aes_key(std::string const& str) -> aes_key<N> {

		BOOST_ASSERT_MSG(str.size() == N / 4, "invalid AES key format");

		auto const buffer = to_buffer(str);
		return make_aes_key<N>(buffer);

	}

}
#endif

