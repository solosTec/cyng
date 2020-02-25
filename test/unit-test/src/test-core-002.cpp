/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#include "test-core-002.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/intrinsics/version.h>

namespace cyng 
{
	bool test_core_002()
	{
// 		16
// 		vffff0001
// 		32
// 		rffffffff00000001
// 		v65535.1
// 		v4294967295.1
		
		version v(0xFFFF, 1);
		BOOST_CHECK_EQUAL(version::size::value, 16);
// 		std::cout << "v" << std::hex << v.full() << std::endl;
		BOOST_CHECK_EQUAL(v.full(), 0xffff0001);

		revision r(0xFFFFFFFF, 1);
		BOOST_CHECK_EQUAL(revision::size::value, 32);
// 		std::cout << "r" << std::hex << r.full() << std::endl;
		BOOST_CHECK_EQUAL(r.full(), 0xffffffff00000001);
		
		version va(0xffff0001);
		BOOST_CHECK_EQUAL(va.major(), 65535);
		BOOST_CHECK_EQUAL(va.minor(), 1);
		
		revision ra(0xFFFFFFFF00000001ull);
// 		std::cout << "v" << std::dec << r.major() << '.' << r.minor() << std::endl;
		BOOST_CHECK (r == ra);
		
		return true;
	}	
}
