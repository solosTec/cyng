/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Sylko Olzscher 
 * 
 */ 
#include "test-core-007.h"
#include <boost/test/unit_test.hpp>
#include <cyng/intrinsics/mac.h>
#include <cstdint>
#include <iostream>

namespace cyng 
{
	bool test_core_007()
	{
		mac48 mac(0x52, 0x74, 0xf2, 0xb1, 0xa8, 0x7f);
		//	 fe80::5074:f2ff:feb1:a87f
		auto const llocal = mac.to_ipv6_link_local();
		//std::cout << llocal << std::endl;
		auto const bytes = llocal.to_bytes();

		BOOST_CHECK_EQUAL(bytes.at(0), 0xFE);
		BOOST_CHECK_EQUAL(bytes.at(1), 0x80);
		BOOST_CHECK_EQUAL(bytes.at(2), 0x00);
		BOOST_CHECK_EQUAL(bytes.at(3), 0x00);
		BOOST_CHECK_EQUAL(bytes.at(4), 0x00);
		BOOST_CHECK_EQUAL(bytes.at(5), 0x00);
		BOOST_CHECK_EQUAL(bytes.at(6), 0x00);
		BOOST_CHECK_EQUAL(bytes.at(7), 0x00);
		BOOST_CHECK_EQUAL(bytes.at(8), 0x50);
		BOOST_CHECK_EQUAL(bytes.at(9), 0x74);
		BOOST_CHECK_EQUAL(bytes.at(10), 0xF2);
		BOOST_CHECK_EQUAL(bytes.at(11), 0xFF);
		BOOST_CHECK_EQUAL(bytes.at(12), 0xFE);
		BOOST_CHECK_EQUAL(bytes.at(13), 0xB1);
		BOOST_CHECK_EQUAL(bytes.at(14), 0xA8);
		BOOST_CHECK_EQUAL(bytes.at(15), 0x7F);

		return true;
	}
}
