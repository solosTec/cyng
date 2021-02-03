#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE unit_test
#endif

#include <boost/test/unit_test.hpp>
#include <cyng/parse/hex.h>
#include <cyng/parse/mac.h>
#include <cyng/io/ostream.h>

#include <iostream>

BOOST_AUTO_TEST_SUITE(parse_suite)

BOOST_AUTO_TEST_CASE(hex)
{
	auto const i1 = cyng::hex_to_int('9', 'F');
	BOOST_REQUIRE_EQUAL(i1, 0x9F);
	// std::cout << std::hex << static_cast<unsigned>(i)<< std::endl;
	auto const i2 = cyng::hex_to_int('0', 'F');
	BOOST_REQUIRE_EQUAL(i2, 0x0F);

	auto const i3 = cyng::hex_to_int('F', '0');
	BOOST_REQUIRE_EQUAL(i3, 0xF0);

	auto const i4 = cyng::hex_to_int("42");
	BOOST_REQUIRE_EQUAL(i4, 0x42);	
}

BOOST_AUTO_TEST_CASE(mac)
{
	cyng::mac48 const cmp(0x00, 0x0c, 0x29, 0xcc, 0x3e, 0xd4);
	auto const mac = cyng::to_mac48("00:0c:29:cc:e3:d4");
	std::cout << mac << ", " << cmp << std::endl;
	BOOST_REQUIRE_EQUAL(cmp, mac);	
}

BOOST_AUTO_TEST_SUITE_END()