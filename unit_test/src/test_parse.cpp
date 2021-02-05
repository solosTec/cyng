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
	auto const i1 = cyng::hex_to_u8('9', 'F');
	BOOST_REQUIRE_EQUAL(i1, 0x9F);

	auto const i2 = cyng::hex_to_u8('0', 'F');
	BOOST_REQUIRE_EQUAL(i2, 0x0F);

	auto const i3 = cyng::hex_to_u8('F', '0');
	BOOST_REQUIRE_EQUAL(i3, 0xF0);

	auto const i4 = cyng::hex_to_u8("42");
	BOOST_REQUIRE_EQUAL(i4, 0x42);	

	auto const i5 = cyng::hex_to_u16('0', '0', '2', 'D');
	BOOST_REQUIRE_EQUAL(i5, 0x2D);

	auto const i6 = cyng::hex_to_u16("D538");
	BOOST_REQUIRE_EQUAL(i6, 0xD538);

}

BOOST_AUTO_TEST_CASE(mac)
{
	cyng::mac48 const cmp48(0x00, 0x0c, 0x29, 0xcc, 0xe3, 0xd4);
	auto const mac1 = cyng::to_mac48("00:0c:29:cc:e3:d4");
	//std::cout << mac << ", " << cmp << std::endl;
	BOOST_REQUIRE_EQUAL(cmp48, mac1);

	cyng::mac64 const cmp64(0x70, 0xB3, 0xD5, 0x38, 0x70, 0x00, 0x00, 0x2D);
	auto const mac2 = cyng::to_mac64("70B3:D538:7000:002D");
	BOOST_REQUIRE_EQUAL(cmp64, mac2);

}

BOOST_AUTO_TEST_SUITE_END()