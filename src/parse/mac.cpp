/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#include <cyng/parse/mac.h>
#include <cyng/parse/hex.h>

#include <algorithm>

#include <boost/assert.hpp>
#include <boost/algorithm/string.hpp>

namespace cyng {

	mac48 to_mac48(std::string const& str) {

		mac48::address_type address{ 0 };
		//
		//	expected format is "00:0c:29:cc:e3:d4"
		//
		std::vector<std::string> parts;
		boost::split(parts, str, boost::is_any_of(":"));

		if (parts.size() == mac48::length_t::value) {

			std::transform(std::begin(parts), std::end(parts), std::begin(address), [](std::string const& part) {
				return hex_to_u8(part);
				});

		}

		return mac48(address);
	}

	mac64 to_mac64(std::string const& str) {

		mac64::address_type address{ 0 };

		//
		//	expected format is "70B3:D538:7000:002D"
		//
		std::vector<std::string> parts;
		boost::split(parts, str, boost::is_any_of(":"));

		if (parts.size() == mac64::length_t::value) {

			std::transform(std::begin(parts), std::end(parts), std::begin(address), [](std::string const& part) {
				return hex_to_u16(part);
				});

		}
		return mac64(address);

	}

}

