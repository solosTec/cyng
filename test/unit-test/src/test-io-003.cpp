
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 
#include "test-io-003.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/io/io_buffer.h>

namespace cyng 
{

	bool test_io_003()
	{
		
		auto s = io::to_hex(0x12345678);
		BOOST_CHECK_EQUAL(s, "12345678");

		s = io::to_hex(0xABCDEF);
		BOOST_CHECK_EQUAL(s, "00ABCDEF");

		s = io::to_hex(48879);
		BOOST_CHECK_EQUAL(s, "0000BEEF");

		char const c{ 1 };
		s = io::to_hex(c);
		BOOST_CHECK_EQUAL(s, "01");

		short const i{ 12 };
		s = io::to_hex(i);
		BOOST_CHECK_EQUAL(s, "000C");
		return true;
	}
	
}
