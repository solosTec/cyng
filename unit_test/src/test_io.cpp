#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE unit_test
#endif

#include <boost/test/unit_test.hpp>
#include <cyng/io/serialize.h>
#include <cyng/io/hex_dump.hpp>
#include <cyng/io/io_buffer.h>
#include <cyng/io/ostream.h>
#include <cyng/io/parser/parser.h>
#include <cyng/obj/factory.hpp>
#include <cyng/obj/container_factory.hpp>
#include <cyng/sys/process.h>
#include <cyng/parse/buffer.h>
#include <cyng/obj/util.hpp>

#include <fstream>

#include <boost/asio.hpp>
#include <boost/uuid/uuid_generators.hpp>

BOOST_AUTO_TEST_SUITE(io_suite)

BOOST_AUTO_TEST_CASE(plain)
{

	//	null
	BOOST_REQUIRE_EQUAL(cyng::io::to_plain(cyng::make_object()), "null");

	//	bool
	BOOST_REQUIRE_EQUAL(cyng::io::to_plain(cyng::make_object(false)), "false");

	//	char

	//float,
	BOOST_REQUIRE_EQUAL(cyng::io::to_plain(cyng::make_object(2.3f)), "2.3");

	//double,
	BOOST_REQUIRE_EQUAL(cyng::io::to_plain(cyng::make_object(2.3)), "2.30");
	//long double,

	//std::uint8_t,
	BOOST_REQUIRE_EQUAL(cyng::io::to_plain(cyng::make_object<std::uint8_t>(0xff)), "ff");

	//std::uint16_t,
	BOOST_REQUIRE_EQUAL(cyng::io::to_plain(cyng::make_object<std::uint16_t>(0xff)), "00ff");
	//std::uint32_t,
	BOOST_REQUIRE_EQUAL(cyng::io::to_plain(cyng::make_object<std::uint32_t>(0xff)), "000000ff");
	//std::uint64_t,
	//std::int8_t,
	//std::int16_t,
	//std::int32_t,
	BOOST_REQUIRE_EQUAL(cyng::io::to_plain(cyng::make_object<std::int32_t>(0xff)), "255");
	//std::int64_t,

	//std::string,
	//std::filesystem::path,
	////std::filesystem::file_status,

	//std::chrono::system_clock::time_point,
	//std::chrono::nanoseconds,
	//std::chrono::microseconds,
	//std::chrono::milliseconds,
	//std::chrono::seconds,
	BOOST_REQUIRE_EQUAL(cyng::io::to_plain(cyng::make_object(std::chrono::seconds(24))), "00:00:24.000000");
	
	//std::chrono::minutes,
	//std::chrono::hours,
	BOOST_REQUIRE_EQUAL(cyng::io::to_plain(cyng::make_object(std::chrono::hours(24))), "24:00:0.000000");

	//buffer_t,
	//version,
	//revision,
	//op,
	//severity,
	//mac48,
	BOOST_REQUIRE_EQUAL(cyng::io::to_plain(cyng::make_object(cyng::broadcast_address())), "ff:ff:ff:ff:ff:ff");
	//mac64,

	//crypto::digest_md5,
	//crypto::digest_sha1,
	//crypto::digest_sha256,
	//crypto::digest_sha512,

	//crypto::aes_128_key,
	//crypto::aes_192_key,
	//crypto::aes_256_key,

	//tuple_t,
	//vector_t,
	//deque_t,

	//attr_map_t,		//	std::map<std::size_t, object>;
	//attr_t,			//	std::pair<std::size_t, object>;
	//param_map_t,	//	std::map<std::string, object>;
	//param_t,		//	std::pair<std::string, object>;

	//boost::system::error_code,
	//boost::uuids::uuid,

	//boost::asio::ip::address,
	//boost::asio::ip::tcp::endpoint,
	//boost::asio::ip::udp::endpoint,
	//boost::asio::ip::icmp::endpoint,

	//	eod
	BOOST_REQUIRE_EQUAL(cyng::io::to_plain(cyng::make_eod()), "eod");
}

BOOST_AUTO_TEST_CASE(typed)
{
	BOOST_REQUIRE_EQUAL(cyng::io::to_typed(cyng::make_object()), "null");

	//std::cout << cyng::io::to_plain(cyng::make_object(false)) << std::endl;
	BOOST_REQUIRE_EQUAL(cyng::io::to_typed(cyng::make_object(false)), "false");
	//char,
	//float,
	BOOST_REQUIRE_EQUAL(cyng::io::to_typed(cyng::make_object(2.3f)), "2.3f");
	//double,
	BOOST_REQUIRE_EQUAL(cyng::io::to_typed(cyng::make_object(2.3)), "2.30d");
	//long double,

	//std::uint8_t,
	BOOST_REQUIRE_EQUAL(cyng::io::to_typed(cyng::make_object<std::uint8_t>(0xff)), "ffu8");
	//std::uint16_t,
	BOOST_REQUIRE_EQUAL(cyng::io::to_typed(cyng::make_object<std::uint16_t>(0xff)), "00ffu16");
	//std::uint32_t,
	BOOST_REQUIRE_EQUAL(cyng::io::to_typed(cyng::make_object<std::uint32_t>(0xff)), "000000ffu32");
	//std::uint64_t,
	BOOST_REQUIRE_EQUAL(cyng::io::to_typed(cyng::make_object<std::uint64_t>(0xff)), "00000000000000ffu64");
	//std::int8_t,
	BOOST_REQUIRE_EQUAL(cyng::io::to_typed(cyng::make_object<std::int8_t>(0x7f)), "127i8");
	//std::int16_t,
	BOOST_REQUIRE_EQUAL(cyng::io::to_typed(cyng::make_object<std::int16_t>(0xff)), "255i16");
	//std::int32_t,
	BOOST_REQUIRE_EQUAL(cyng::io::to_typed(cyng::make_object<std::int32_t>(0xff)), "255i32");
	//std::int64_t,
	BOOST_REQUIRE_EQUAL(cyng::io::to_typed(cyng::make_object<std::int64_t>(0xff)), "255i64");

	//std::string,
	//std::filesystem::path,
	////std::filesystem::file_status,

	//std::chrono::system_clock::time_point,
	//std::chrono::nanoseconds,
	//std::chrono::microseconds,
	//std::chrono::milliseconds,
	//std::chrono::seconds,
	//std::chrono::minutes,
	//std::chrono::hours,

	//buffer_t,
	//version,
	//revision,
	//op,
	//severity,
	//mac48,
	//mac64,

	//crypto::digest_md5,
	//crypto::digest_sha1,
	//crypto::digest_sha256,
	//crypto::digest_sha512,

	//crypto::aes_128_key,
	//crypto::aes_192_key,
	//crypto::aes_256_key,

	//tuple_t,
	//vector_t,
	//deque_t,

	//attr_map_t,		//	std::map<std::size_t, object>;
	//attr_t,			//	std::pair<std::size_t, object>;
	//param_map_t,	//	std::map<std::string, object>;
	//param_t,		//	std::pair<std::string, object>;

	//boost::system::error_code,
	//boost::uuids::uuid,

	//boost::asio::ip::address,
	//boost::asio::ip::tcp::endpoint,
	//boost::asio::ip::udp::endpoint,
	//boost::asio::ip::icmp::endpoint,

	BOOST_REQUIRE_EQUAL(cyng::io::to_typed(cyng::make_eod()), "eod");
}

BOOST_AUTO_TEST_CASE(binary)
{
}

BOOST_AUTO_TEST_CASE(json)
{
	// std::cout << cyng::io::to_json(cyng::make_object(false)) << std::endl;
	BOOST_REQUIRE_EQUAL(cyng::io::to_json(cyng::make_object(false)), "false");
	//std::cout << cyng::io::to_json(cyng::make_object(std::filesystem::path("readme.txt"))) << std::endl;
	BOOST_REQUIRE_EQUAL(cyng::io::to_json(cyng::make_object(std::filesystem::path("readme.txt"))), "\"readme.txt\"");
	//	1974-02-09T00:00:00+0100
	//auto const tp = std::chrono::system_clock::time_point(std::chrono::hours(36000));
	//std::cout << cyng::io::to_json(cyng::make_object(tp));
	BOOST_REQUIRE_EQUAL(cyng::io::to_json(cyng::make_object(std::chrono::system_clock::time_point(std::chrono::hours(36000)))), "\"1974-02-09T00:00:00+0100\"");

	//	the generic "Jan. 1 1970" is null
	BOOST_REQUIRE_EQUAL(cyng::io::to_json(cyng::make_object(std::chrono::system_clock::time_point(std::chrono::hours(0)))), "null");

	auto const key_1 = cyng::make_aes_key<cyng::crypto::aes128_size>(cyng::hex_to_buffer("6B59703373367639792F423F4528482B"));
	//std::cout << cyng::io::to_json(cyng::make_object(key_1)) << std::endl;
	BOOST_REQUIRE_EQUAL(cyng::io::to_json(cyng::make_object(key_1)), "\"6b59703373367639792f423f4528482b\"");
	//std::cout << cyng::io::to_json(cyng::make_object(cyng::pid(12345))) << std::endl;
	BOOST_REQUIRE_EQUAL(cyng::io::to_json(cyng::make_object(cyng::pid(12345))), "\"012345\"");

	BOOST_REQUIRE_EQUAL(cyng::io::to_json(cyng::make_object(cyng::severity::LEVEL_WARNING)), "\"WARNING\"");


}

BOOST_AUTO_TEST_CASE(csv)
{
	//std::cout << cyng::io::to_csv(cyng::make_object(false)) << std::endl;
	BOOST_REQUIRE_EQUAL(cyng::io::to_csv(cyng::make_object(false)), "false");
}

BOOST_AUTO_TEST_CASE(dump)
{
	std::stringstream ss;
	cyng::io::hex_dump<8> hd;
	std::string const str("hello, world!");
	hd(ss, std::begin(str), std::end(str));
	auto const r = ss.str();
	BOOST_REQUIRE_EQUAL(r, std::string("[0000]  68 65 6c 6c 6f 2c 20 77  6f 72 6c 64 21           hello, w orld!\n"));
}

BOOST_AUTO_TEST_CASE(buffer)
{
	std::uint32_t n = 0x01020304;
	BOOST_REQUIRE_EQUAL(cyng::io::to_hex(n), std::string("01020304"));

	BOOST_REQUIRE_EQUAL(cyng::io::to_hex({ 1, 2, 3, 4, 127 }, ','), std::string("01,02,03,04,7f"));

}

cyng::buffer_t convert(cyng::object const& obj) {

	boost::asio::streambuf	stream_buffer;
	std::iostream			input(&stream_buffer);

	auto const size = cyng::io::serialize_binary(input, obj);
	cyng::buffer_t buffer;
	buffer.resize(size);
	input.read(buffer.data(), size);
	return buffer;
}


BOOST_AUTO_TEST_CASE(parser)
{
	std::string cmp;

	cyng::io::parser p([&cmp](cyng::object&& obj) {
		using cyng::operator<<;
		//std::cout << obj << std::endl;
		auto const str = cyng::io::to_typed(obj);
		//std::cout << str << std::endl;
		BOOST_REQUIRE_EQUAL(cmp, str);
		});

	//
	//	test int (std::int32_t)
	//
	auto obj = cyng::make_object(std::numeric_limits<std::int32_t>::max());	//	0x7FFFFFFF (2147483647)
	cmp = cyng::io::to_typed(obj);	//	2147483647i32
	auto inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test "hello, world!"
	//
	obj = cyng::make_object("hello, world!");
	cmp = cyng::io::to_typed(obj);	//	hello, world!:s
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test "¶" c2 b6
	//
	obj = cyng::make_object(std::string("\xc2\xb6"));
	cmp = cyng::io::to_typed(obj);	//	utf-8
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test 7.2e12
	//
	obj = cyng::make_object(7.2e12);
	cmp = cyng::io::to_typed(obj);	//	7200000000000.00d
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test broadcast_address
	//
	obj = cyng::make_object(cyng::broadcast_address());
	cmp = cyng::io::to_typed(obj);	//	ff:ff:ff:ff:ff:ff:mac48
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test uuid
	//
	obj = cyng::make_object(boost::uuids::uuid{ {0x7c, 0xa6, 0xb0, 0x5c, 0x2a, 0x02, 0x47, 0x55, 0x9e, 0xc5, 0xe6, 0xd0, 0x6a, 0x45, 0x89, 0xdd} });
	cmp = cyng::io::to_typed(obj);	//	c59e:d0e6:456a:dd89:uuid
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test long string
	//
	std::string s(512, 'q');	//	512 => 7e 02
	obj = cyng::make_object(s);
	cmp = cyng::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test timestamp
	//
	obj = cyng::make_object(std::chrono::system_clock::now());
	cmp = cyng::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test filesystem path
	//
	obj = cyng::make_object(std::filesystem::temp_directory_path());
	cmp = cyng::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test microseconds
	//
	obj = cyng::make_object(std::chrono::microseconds(499));
	cmp = cyng::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test milliseconds
	//
	obj = cyng::make_object(std::chrono::milliseconds(372));
	cmp = cyng::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test seconds
	//
	obj = cyng::make_object(std::chrono::seconds(372));
	cmp = cyng::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test minutes
	//
	obj = cyng::make_object(std::chrono::minutes(62));
	cmp = cyng::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test hours
	//
	obj = cyng::make_object(std::chrono::hours(25));
	cmp = cyng::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test ip address
	//
	obj = cyng::make_object(boost::asio::ip::make_address("1.2.3.4"));
	cmp = cyng::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test tcp/ip endpoint IPv4
	//
	obj = cyng::make_object(boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address("1.2.3.4"), 1024u));
	cmp = cyng::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test tcp/ip endpoint IPv6
	//
	obj = cyng::make_object(boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address("[2001:171b:226b:a7d0:225:18ff:fea4:9982]"), 7001u));
	cmp = cyng::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test system error
	//
	obj = cyng::make_object(boost::system::errc::make_error_code(boost::system::errc::io_error));
	//obj = cyng::make_object(boost::asio::error::make_error_code(boost::asio::error::network_down));
	cmp = cyng::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test null value
	//
	obj = cyng::make_object();
	cmp = cyng::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test eod value
	//
	obj = cyng::make_eod();
	cmp = cyng::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test op
	//
	obj = cyng::make_object(cyng::op::NOOP);
	cmp = cyng::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test severity
	//
	obj = cyng::make_object(cyng::severity::LEVEL_WARNING);
	cmp = cyng::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test version
	//
	obj = cyng::make_object(cyng::version(3, 11));
	cmp = cyng::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test revision
	//
	obj = cyng::make_object(cyng::revision(3, 11, 0, 2078));
	cmp = cyng::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test obis
	//
	obj = cyng::make_object(cyng::obis(0x01, 0x00, 0x00, 0x00, 0x09, 0xff));
	cmp = cyng::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test obis path
	//
	obj = cyng::make_object(cyng::obis_path_t{ cyng::obis(0x01, 0x00, 0x00, 0x00, 0x09, 0xff), cyng::obis(0x01, 0x00, 0x00, 0x00, 0x09, 0x01) });
	cmp = cyng::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test edis
	//
	obj = cyng::make_object(cyng::edis(1, 8, 0));
	cmp = cyng::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test PID
	//
	obj = cyng::make_object(cyng::sys::get_process_id());
	cmp = cyng::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	AES key 128
	//
	auto const key_128 = cyng::make_aes_key<cyng::crypto::aes128_size>(cyng::hex_to_buffer("6B59703373367639792F423F4528482B"));
	obj = cyng::make_object(key_128);
	cmp = cyng::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	color_8
	//
	obj = cyng::make_object(cyng::color_8(0, 128, 64, 0));
	cmp = cyng::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	param_t
	//
	//obj = cyng::pair_factory("abc", "hello");
	//cmp = cyng::io::to_typed(obj);	//	
	//inp = convert(obj);
	//p.read(std::begin(inp), std::end(inp));

}

BOOST_AUTO_TEST_CASE(path)
{
	auto const s = cyng::to_path('/', 1, "hello", cyng::version(3, 11), cyng::obis(1, 2, 3, 4, 5, 0xFF), cyng::edis(0, 9, 2));
	//std::cout << s << std::endl;
	BOOST_REQUIRE_EQUAL(s, "1/hello/3.11/0102030405ff/0.9.2");

}

BOOST_AUTO_TEST_CASE(xml)
{
	auto const tp = std::chrono::system_clock::from_time_t(1642664487);
	boost::uuids::uuid tag{ {0x7c, 0xa6, 0xb0, 0x5c, 0x2a, 0x02, 0x47, 0x55, 0x9e, 0xc5, 0xe6, 0xd0, 0x6a, 0x45, 0x89, 0xdd} };
	auto const vec = cyng::make_vector({ cyng::make_tuple(
		cyng::make_param("generated", tp),
		cyng::make_param("version", cyng::version(1, 2)),
		cyng::make_param("log-dir", "/tmp"),
		cyng::make_param("tag", tag),
		cyng::make_param("country-code", "CH"),
		cyng::make_param("language-code", "AA"),
		cyng::make_param("generate-profile", false)) });

	auto const str = cyng::io::to_xml(cyng::make_object(vec), "root");
	//std::cout << str << std::endl;
	//
	//	note: The comparison string contains tabs '\t'. The end of line character is '\n'.
	//	This could be a problem on different platforms with a different end-of-line marker.
	//
	std::string r = R"xml(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<root xmlns:xsi="w3.org/2001/XMLSchema-instance" type="vec" size="1">
	<element index="0" type="tpl" size="7">
		<param name="generated" type="chrono:tp">2022-01-20T07:41:27+0100</param>
		<param name="version" type="v">1.2</param>
		<param name="log-dir" type="s">/tmp</param>
		<param name="tag" type="uuid">7ca6b05c-2a02-4755-9ec5-e6d06a4589dd</param>
		<param name="country-code" type="s">CH</param>
		<param name="language-code" type="s">AA</param>
		<param name="generate-profile" type="bool">false</param>
	</element>
</root>
)xml";
	BOOST_CHECK_EQUAL(r, str);
}

BOOST_AUTO_TEST_CASE(cpp)
{	
	auto const vec = cyng::make_vector({ cyng::make_tuple(
		cyng::make_param("generated", std::chrono::system_clock::now()),
		cyng::make_param("version", cyng::version(1, 2)),
		cyng::make_param("log-dir", "/tmp"),
		cyng::make_param("tag", boost::uuids::random_generator()()),
		cyng::make_param("country-code", "CH"),
		cyng::make_param("language-code", "AA"),
		cyng::make_param("generate-profile", false)) });

	auto const str = cyng::io::to_cpp(cyng::make_object(vec));
	//std::cout << str << std::endl;

}

BOOST_AUTO_TEST_SUITE_END()