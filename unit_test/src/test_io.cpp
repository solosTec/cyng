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

#include <fstream>

#include <boost/asio.hpp>

BOOST_AUTO_TEST_SUITE(io_suite)

BOOST_AUTO_TEST_CASE(plain)
{

	//	null
	BOOST_REQUIRE_EQUAL(docscript::io::to_plain(docscript::make_object()), "null");

	//	bool
	BOOST_REQUIRE_EQUAL(docscript::io::to_plain(docscript::make_object(false)), "false");

	//	char

	//float,
	BOOST_REQUIRE_EQUAL(docscript::io::to_plain(docscript::make_object(2.3f)), "2.3");

	//double,
	BOOST_REQUIRE_EQUAL(docscript::io::to_plain(docscript::make_object(2.3)), "2.30");
	//long double,

	//std::uint8_t,
	BOOST_REQUIRE_EQUAL(docscript::io::to_plain(docscript::make_object<std::uint8_t>(0xff)), "ff");

	//std::uint16_t,
	BOOST_REQUIRE_EQUAL(docscript::io::to_plain(docscript::make_object<std::uint16_t>(0xff)), "00ff");
	//std::uint32_t,
	BOOST_REQUIRE_EQUAL(docscript::io::to_plain(docscript::make_object<std::uint32_t>(0xff)), "000000ff");
	//std::uint64_t,
	//std::int8_t,
	//std::int16_t,
	//std::int32_t,
	BOOST_REQUIRE_EQUAL(docscript::io::to_plain(docscript::make_object<std::int32_t>(0xff)), "255");
	//std::int64_t,

	//std::string,
	//std::filesystem::path,
	////std::filesystem::file_status,

	//std::chrono::system_clock::time_point,
	//std::chrono::nanoseconds,
	//std::chrono::microseconds,
	//std::chrono::milliseconds,
	//std::chrono::seconds,
	BOOST_REQUIRE_EQUAL(docscript::io::to_plain(docscript::make_object(std::chrono::seconds(24))), "00:00:24.000000");
	
	//std::chrono::minutes,
	//std::chrono::hours,
	BOOST_REQUIRE_EQUAL(docscript::io::to_plain(docscript::make_object(std::chrono::hours(24))), "24:00:0.000000");

	//buffer_t,
	//version,
	//revision,
	//op,
	//severity,
	//mac48,
	BOOST_REQUIRE_EQUAL(docscript::io::to_plain(docscript::make_object(docscript::broadcast_address())), "ff:ff:ff:ff:ff:ff");
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
	BOOST_REQUIRE_EQUAL(docscript::io::to_plain(docscript::make_eod()), "eod");
}

BOOST_AUTO_TEST_CASE(typed)
{
	BOOST_REQUIRE_EQUAL(docscript::io::to_typed(docscript::make_object()), "null");

	//std::cout << docscript::io::to_plain(docscript::make_object(false)) << std::endl;
	BOOST_REQUIRE_EQUAL(docscript::io::to_typed(docscript::make_object(false)), "false");
	//char,
	//float,
	BOOST_REQUIRE_EQUAL(docscript::io::to_typed(docscript::make_object(2.3f)), "2.3f");
	//double,
	BOOST_REQUIRE_EQUAL(docscript::io::to_typed(docscript::make_object(2.3)), "2.30d");
	//long double,

	//std::uint8_t,
	BOOST_REQUIRE_EQUAL(docscript::io::to_typed(docscript::make_object<std::uint8_t>(0xff)), "ffu8");
	//std::uint16_t,
	BOOST_REQUIRE_EQUAL(docscript::io::to_typed(docscript::make_object<std::uint16_t>(0xff)), "00ffu16");
	//std::uint32_t,
	BOOST_REQUIRE_EQUAL(docscript::io::to_typed(docscript::make_object<std::uint32_t>(0xff)), "000000ffu32");
	//std::uint64_t,
	BOOST_REQUIRE_EQUAL(docscript::io::to_typed(docscript::make_object<std::uint64_t>(0xff)), "00000000000000ffu64");
	//std::int8_t,
	BOOST_REQUIRE_EQUAL(docscript::io::to_typed(docscript::make_object<std::int8_t>(0x7f)), "127i8");
	//std::int16_t,
	BOOST_REQUIRE_EQUAL(docscript::io::to_typed(docscript::make_object<std::int16_t>(0xff)), "255i16");
	//std::int32_t,
	BOOST_REQUIRE_EQUAL(docscript::io::to_typed(docscript::make_object<std::int32_t>(0xff)), "255i32");
	//std::int64_t,
	BOOST_REQUIRE_EQUAL(docscript::io::to_typed(docscript::make_object<std::int64_t>(0xff)), "255i64");

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

	BOOST_REQUIRE_EQUAL(docscript::io::to_typed(docscript::make_eod()), "eod");
}

BOOST_AUTO_TEST_CASE(binary)
{
}

BOOST_AUTO_TEST_CASE(json)
{
	// std::cout << docscript::io::to_json(docscript::make_object(false)) << std::endl;
	BOOST_REQUIRE_EQUAL(docscript::io::to_json(docscript::make_object(false)), "false");
}

BOOST_AUTO_TEST_CASE(csv)
{
	// std::cout << docscript::io::to_csv(docscript::make_object(false)) << std::endl;
	BOOST_REQUIRE_EQUAL(docscript::io::to_csv(docscript::make_object(false)), "false");
}

BOOST_AUTO_TEST_CASE(dump)
{
	std::stringstream ss;
	docscript::io::hex_dump<8> hd;
	std::string const str("hello, world!");
	hd(ss, std::begin(str), std::end(str));
	auto const r = ss.str();
	BOOST_REQUIRE_EQUAL(r, std::string("[0000]  68 65 6c 6c 6f 2c 20 77  6f 72 6c 64 21           hello, w orld!\n"));
}

BOOST_AUTO_TEST_CASE(buffer)
{
	std::uint32_t n = 0x01020304;
	BOOST_REQUIRE_EQUAL(docscript::io::to_hex(n), std::string("01020304"));

	BOOST_REQUIRE_EQUAL(docscript::io::to_hex({ 1, 2, 3, 4, 127 }, ','), std::string("01,02,03,04,7f"));

}

docscript::buffer_t convert(docscript::object const& obj) {

	boost::asio::streambuf	stream_buffer;
	std::iostream			input(&stream_buffer);

	auto const size = docscript::io::serialize_binary(input, obj);
	docscript::buffer_t buffer;
	buffer.resize(size);
	input.read(buffer.data(), size);
	return buffer;
}


BOOST_AUTO_TEST_CASE(parser)
{
	std::string cmp;

	docscript::io::parser p([&cmp](docscript::object&& obj) {
		using docscript::operator<<;
		//std::cout << obj << std::endl;
		auto const str = docscript::io::to_typed(obj);
		// std::cout << str << std::endl;
		BOOST_REQUIRE_EQUAL(cmp, str);
		});

	//
	//	test int (std::int32_t)
	//
	auto obj = docscript::make_object(std::numeric_limits<std::int32_t>::max());	//	0x7FFFFFFF (2147483647)
	cmp = docscript::io::to_typed(obj);	//	2147483647i32
	auto inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test "hello, world!"
	//
	obj = docscript::make_object("hello, world!");
	cmp = docscript::io::to_typed(obj);	//	hello, world!:s
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test 7.2e12
	//
	obj = docscript::make_object(7.2e12);
	cmp = docscript::io::to_typed(obj);	//	7200000000000.00d
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test broadcast_address
	//
	obj = docscript::make_object(docscript::broadcast_address());
	cmp = docscript::io::to_typed(obj);	//	ff:ff:ff:ff:ff:ff:mac48
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test uuid
	//
	obj = docscript::make_object(boost::uuids::uuid{ {0x7c, 0xa6, 0xb0, 0x5c, 0x2a, 0x02, 0x47, 0x55, 0x9e, 0xc5, 0xe6, 0xd0, 0x6a, 0x45, 0x89, 0xdd} });
	cmp = docscript::io::to_typed(obj);	//	c59e:d0e6:456a:dd89:uuid
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test long string
	//
	std::string s(512, 'q');	//	512 => 7e 02
	obj = docscript::make_object(s);
	cmp = docscript::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test timestamp
	//
	obj = docscript::make_object(std::chrono::system_clock::now());
	cmp = docscript::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test filesystem path
	//
	obj = docscript::make_object(std::filesystem::temp_directory_path());
	cmp = docscript::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test microseconds
	//
	obj = docscript::make_object(std::chrono::microseconds(499));
	cmp = docscript::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test milliseconds
	//
	obj = docscript::make_object(std::chrono::milliseconds(372));
	cmp = docscript::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test seconds
	//
	obj = docscript::make_object(std::chrono::seconds(372));
	cmp = docscript::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test minutes
	//
	obj = docscript::make_object(std::chrono::minutes(62));
	cmp = docscript::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test hours
	//
	obj = docscript::make_object(std::chrono::hours(25));
	cmp = docscript::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test ip address
	//
	obj = docscript::make_object(boost::asio::ip::make_address("1.2.3.4"));
	cmp = docscript::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test tcp/ip endpoint
	//
	obj = docscript::make_object(boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address("1.2.3.4"), 1024u));
	cmp = docscript::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test system error
	//
	obj = docscript::make_object(boost::system::errc::make_error_code(boost::system::errc::io_error));
	//obj = docscript::make_object(boost::asio::error::make_error_code(boost::asio::error::network_down));
	cmp = docscript::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test null value
	//
	obj = docscript::make_object();
	cmp = docscript::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test eod value
	//
	obj = docscript::make_eod();
	cmp = docscript::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test op
	//
	obj = docscript::make_object(docscript::op::NOOP);
	cmp = docscript::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test severity
	//
	obj = docscript::make_object(docscript::severity::LEVEL_WARNING);
	cmp = docscript::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test version
	//
	obj = docscript::make_object(docscript::version(3, 11));
	cmp = docscript::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

	//
	//	test revision
	//
	obj = docscript::make_object(docscript::revision(3, 11, 0, 2078));
	cmp = docscript::io::to_typed(obj);	//	
	inp = convert(obj);
	p.read(std::begin(inp), std::end(inp));

}

BOOST_AUTO_TEST_SUITE_END()