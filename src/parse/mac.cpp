/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#include <cyng/parse/mac.h>
#include <cyng/parse/hex.h>

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

		std::size_t idx{ 0 };
		for(auto const& part : parts) {

			address[idx] = hex_to_int(part);
			idx++;
		}

		return mac48(address);
	}
}

