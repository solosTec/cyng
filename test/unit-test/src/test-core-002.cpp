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
		version v(0xFFFF, 1);
		std::cout << std::dec << version::size::value << std::endl;
		std::cout << "v" << std::hex << v.full() << std::endl;

		revision r(0xFFFFFFFF, 1);
		std::cout << std::dec << revision::size::value << std::endl;
		std::cout << "r" << std::hex << r.full() << std::endl;
		
		version va(0xffff0001);
		std::cout << "v" << std::dec << v.major() << '.' << v.minor() << std::endl;
		
		revision ra(0xFFFFFFFF00000001ull);
		std::cout << "v" << std::dec << r.major() << '.' << r.minor() << std::endl;
		BOOST_CHECK (r == ra);
		
		return true;
	}	
}
