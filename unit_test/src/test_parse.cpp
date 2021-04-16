#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE unit_test
#endif

#include <boost/test/unit_test.hpp>
#include <cyng/parse/hex.h>
#include <cyng/parse/buffer.h>
#include <cyng/parse/mac.h>
#include <cyng/parse/net.h>

#include <cyng/io/ostream.h>
#include <cyng/io/serialize.h>
#include <cyng/parse/json/json_parser.h>
#include <cyng/parse/csv/csv_parser.h>
#include <cyng/parse/csv/line_cast.hpp>
#include <cyng/parse/csv.h>

#include <cyng/obj/tag.hpp>
#include <cyng/obj/util.hpp>
#include <cyng/obj/container_cast.hpp>

#include <iostream>
#include <iomanip>
#include <fstream>

#include <boost/asio.hpp>

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
	cyng::csv::parser csvp1(',', [](cyng::csv::line_t&& line) {
		//for (auto const& s : line) {
		//	std::cout << '"' << s << '"' << std::endl;
		//}
		//std::cout << cyng::to_string(vec) << std::endl;
		//BOOST_REQUIRE_EQUAL(cyng::to_string(vec), "[name-1,null,name-2,name-3,1,2,3,true,false,4200.00]");
		auto const tpl = cyng::csv::line_cast<std::string
			, std::string
			, std::string
			, std::uint32_t
			, std::uint16_t
			, std::int32_t
			, bool
			, bool
			, double>(line);

		BOOST_REQUIRE_EQUAL(std::get<0>(tpl), "name-1");
		BOOST_REQUIRE_EQUAL(std::get<1>(tpl), "name-2");
		BOOST_REQUIRE_EQUAL(std::get<2>(tpl), "name-3");

		BOOST_REQUIRE_EQUAL(std::get<3>(tpl), 1u);
		BOOST_REQUIRE_EQUAL(std::get<4>(tpl), 2u);
		BOOST_REQUIRE_EQUAL(std::get<5>(tpl), 3);

		BOOST_REQUIRE_EQUAL(std::get<6>(tpl), true);
		BOOST_REQUIRE_EQUAL(std::get<7>(tpl), false);

		});

	csvp1.read(std::begin(inp_01), std::end(inp_01));

	//	simulate BOM
	auto inp_02 = std::string("qqqCH0000000000000000000000003218421,RS485,192.168.0.200,6006,Elster,Elster AS 1440,IEC 62056,Lucerne,Office 2,Yes,,,");
	inp_02.at(0) = 0xef;
	inp_02.at(1) = 0xbb;
	inp_02.at(2) = 0xbf;
	cyng::csv::parser csvp2(',', [](cyng::csv::line_t&& vec) {
		//for (auto const& s : vec) {
		//	std::cout << '"' << s << '"' << std::endl;
		//}
		//std::cout << cyng::to_string(vec) << std::endl;
		//auto const s = cyng::to_string(vec);
		//	                    [CH0000000000000000000000003218421,RS485,192.168.0.200,6006,Elster,Elster AS 1440,IEC 62056,Lucerne,Office 2,Yes,null,null]
		//BOOST_REQUIRE_EQUAL(s, "[CH0000000000000000000000003218421,RS485,192.168.0.200,6006,Elster,Elster AS 1440,IEC 62056,Lucerne,Office 2,Yes,null,null]");

		});

	csvp2.read(std::begin(inp_02), std::end(inp_02));

	auto const inp_03 = std::string("\"name-1\", \"name-2\", \"name-3\", 1, 2, 3, true, false, 4.2e3");
	cyng::csv::parse_typed<std::string
		, std::string
		, std::string
		, std::uint32_t
		, std::uint16_t
		, std::int32_t
		, bool
		, bool
		, double>(inp_03, [](std::string s1
		, std::string
		, std::string
		, std::uint32_t
		, std::uint16_t
		, std::int32_t
		, bool
		, bool
		, double) {
			std::cout << s1 << std::endl;
		}, true);

	auto const inp_04 = std::string(",,,,,");
	cyng::csv::parser csvp4(',', [](cyng::csv::line_t&& vec) {
		std::cout << std::endl;
		});
	csvp4.read(std::begin(inp_04), std::end(inp_04));
}

//void read_ipv6_info(std::function<void(std::string, std::string, std::uint64_t, std::uint64_t, std::uint64_t, std::uint64_t)> cb) {
//	std::ifstream ifs("D:\\reboot\\cyng\\build\\if_inet6.txt");
//	ifs >> std::setbase(16);
//	std::string address, name;
//	std::uint64_t index, len, scope, flag;
//	while (ifs) {
//		ifs >> address >> index >> len >> scope >> flag >> name;
//		cb(address, name, index, len, scope, flag);
//	}
//}

BOOST_AUTO_TEST_CASE(ipv6)
{
	//cat / proc / net / if_inet6
	//00000000000000000000000000000001 01 80 10 80       lo
	//fe80000000000000022518fffef11fd1 05 40 20 80      br0
	//fe80000000000000022518fffef11fd3 04 40 20 80     eth2
	//2a001e8000640006022518fffef11fd1 05 40 00 00      br0

	boost::system::error_code ec;
	boost::asio::ip::address_v6::bytes_type t;
	//boost::asio::detail::array<unsigned char, 16>
	auto addr = cyng::hex_to_buffer("2a001e8000640006022518fffef11fd1");
	BOOST_REQUIRE_EQUAL(addr.size(), 16);
	auto pos = addr.begin();
	for (auto& e : t) {
		e = *pos++;
	}
	//	2a00:1e80:64:6:225:18ff:fef1:1fd1
	//std::cout << boost::asio::ip::make_address_v6(t, 0).to_string() << std::endl;
	BOOST_REQUIRE_EQUAL(boost::asio::ip::make_address_v6(t, 0).to_string(), "2a00:1e80:64:6:225:18ff:fef1:1fd1");
	//BOOST_TEST(!ec);

	//read_ipv6_info([](std::string address, std::string name, std::uint64_t index, std::uint64_t len, std::uint64_t scope, std::uint64_t flag) {
	//	
	//	std::cout << address << " - " << name << std::endl;

	//	std::cout << cyng::to_ipv6(address, scope) << std::endl;;
	//	});
}

BOOST_AUTO_TEST_SUITE_END()