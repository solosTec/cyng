/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#include "test-core-006.h"
#include <boost/test/unit_test.hpp>
#include <cyng/util/slice.hpp>
#include <cstdint>


namespace cyng 
{
	bool test_core_006()
	{
		std::uint32_t n{ 0xFEFAFEFA };
		auto const a = to_array<char>(n);
		
		BOOST_CHECK_EQUAL((a.at(0) & 0xFF), 0xFE);
		BOOST_CHECK_EQUAL((a.at(1) & 0xFF), 0xFA);
		BOOST_CHECK_EQUAL((a.at(2) & 0xFF), 0xFE);
		BOOST_CHECK_EQUAL((a.at(3) & 0xFF), 0xFA);

		auto v = to_vector<char>(n);
		std::reverse(v.begin(), v.end());

		BOOST_CHECK_EQUAL((v.at(0) & 0xFF), 0xFE);
		BOOST_CHECK_EQUAL((v.at(1) & 0xFF), 0xFA);
		BOOST_CHECK_EQUAL((v.at(2) & 0xFF), 0xFE);
		BOOST_CHECK_EQUAL((v.at(3) & 0xFF), 0xFA);

		return true;
	}
}
