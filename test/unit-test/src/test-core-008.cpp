/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Sylko Olzscher 
 * 
 */ 
#include "test-core-008.h"
#include <boost/test/unit_test.hpp>
#include <cyng/buffer_cast.h>
#include <cyng/io/io_buffer.h>
#include <cstdint>
#include <iostream>

namespace cyng 
{
	bool test_core_008()
	{
		auto const r = to_buffer(0x3105c);
		BOOST_CHECK_EQUAL(r.size(), 4);
		//	5c100300
		// std::cout << io::to_hex(r) << std::endl;
		BOOST_CHECK_EQUAL(r.at(0), 0x5c);
		BOOST_CHECK_EQUAL(r.at(1), 0x10);
		BOOST_CHECK_EQUAL(r.at(2), 0x03);
		BOOST_CHECK_EQUAL(r.at(3), 0x00);
		return true;
	}
}
