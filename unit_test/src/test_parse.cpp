#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE unit_test
#endif

#include <boost/test/unit_test.hpp>
#include <cyng/parse/hex.h>
#include <cyng/parse/mac.h>
#include <cyng/io/ostream.h>
#include <cyng/parse/json/json_parser.h>
#include <cyng/parse/csv/csv_parser.h>
#include <cyng/obj/tag.hpp>
#include <cyng/obj/util.hpp>
#include <cyng/obj/container_cast.hpp>

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

BOOST_AUTO_TEST_CASE(json)
{
	//	\u03BB
	//	\u03AC
	//	\u03BC
	//	\u03B4
	//	\u03B1
	auto const inp_01 = std::string("{ \"id\":5, \"subscription\" : \"/data:localhost:26862\", \"λάμδα\" : true, \"channel\" : \"/meta/subscribe\", \"tag\": \"aee65c3b-080f-41be-b864-ec5c5702c477\" }");

	cyng::json::parser jp1([](cyng::object&& obj) {
		std::cout << obj << std::endl;
		BOOST_CHECK(cyng::is_of_type<cyng::TC_PARAM_MAP>(obj));
		auto const pm = cyng::container_cast<cyng::param_map_t>(obj);
		BOOST_REQUIRE_EQUAL(pm.size(), 5);
		});

	jp1.read(std::begin(inp_01), std::end(inp_01));


	//  
	//  the UTF-8 sequence from key4 results in
	//  z\u00df\u6c34\u1f34c or
	//  0x7a 0xc3, 0x9f 0xe6 0xb0, 0xb4 0xf0 0x9f 0x8d 0x8c
	//  
	auto const s2 = std::string(R"(
        {
            "key-0": [12,22.0,3e-10,[4, 5],[],6],
            "key-1": "value\t1",
            "key-2": "value\u2514",
            "key-3": [
                { "key4": "zß水🍌" },
                null,
                true,
                { "key5a": "value5a", "key5b": "value5b"}
            ],
            "key-6": false,
            "key-7": [1,2,3,4, 5],
            "key-8": "eof"
        }
		)");
	cyng::json::parser jp2([](cyng::object&& obj) {
		std::cout << obj << std::endl;
		BOOST_CHECK(cyng::is_of_type<cyng::TC_PARAM_MAP>(obj));
		auto const pm = cyng::container_cast<cyng::param_map_t>(obj);
		BOOST_REQUIRE_EQUAL(pm.size(), 7);

		});
	jp2.read(std::begin(s2), std::end(s2));

}

BOOST_AUTO_TEST_CASE(csv)
{
	auto const inp_01 = std::string("\"name-1\", \"name-2\", \"name-3\", 1, 2, 3, true, false, 4.2e3");
	cyng::csv::parser csvp1(',', [](cyng::vector_t&& vec) {
		//std::cout << vec << std::endl;
		//std::cout << cyng::to_string(vec) << std::endl;
		//	"[name-1,name-2,name-3,1,2,3,true,false,4200.00]"
		BOOST_REQUIRE_EQUAL(cyng::to_string(vec), "[name-1,name-2,name-3,1,2,3,true,false,4200.00]");

		});

	csvp1.read(std::begin(inp_01), std::end(inp_01));

}

BOOST_AUTO_TEST_SUITE_END()