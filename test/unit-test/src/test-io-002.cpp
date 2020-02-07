
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#include "test-io-002.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/factory.h>
#include <cyng/type.h>
#include <cyng/crypto/hash/md5.h>
#include <cyng/crypto/hash/sha1.h>
#include <cyng/crypto/hash/sha256.h>
#include <cyng/crypto/hash/sha512.h>
#include <cyng/io/serializer.h>
#include <cyng/io/parser/parser.h>
#include <boost/uuid/string_generator.hpp>
#include <boost/math/constants/constants.hpp>

namespace cyng
{
	//
	//	define a custom object
	//
	class custom
	{
	public:
		custom()
		{}
	};

	namespace traits
	{
		template <>
		struct type_tag<custom>
		{
			using type = custom;
			using tag = std::integral_constant<std::size_t, 
#if defined(__CPP_SUPPORT_N2347)
				static_cast<std::size_t>(traits::predef_type_code::PREDEF_CUSTOM)
#else
				PREDEF_CUSTOM
#endif
			>;

#if defined(__CPP_SUPPORT_N2235)
			constexpr static char name[] = "custom";
#else
			const static char name[];
#endif
		};

#if !defined(__CPP_SUPPORT_N2235)
		const char type_tag<custom>::name[] = "custom";
#endif

		template <>
		struct reverse_type < 
#if defined(__CPP_SUPPORT_N2347)
			static_cast<std::size_t>(traits::predef_type_code::PREDEF_CUSTOM)
#else
			PREDEF_CUSTOM 
#endif
		>
		{
			using type = custom;
		};
	}
}

#include <functional>
#include <boost/functional/hash.hpp>

namespace std
{

	template<>
	struct hash<cyng::custom>
	{
		inline size_t operator()(cyng::custom const& s) const noexcept
		{
			return 0;
		}
	};
	template<>
	struct equal_to<cyng::custom>
	{
		using result_type = bool;
		using first_argument_type = cyng::custom;
		using second_argument_type = cyng::custom;

		inline bool operator()(cyng::custom const& s1, cyng::custom const& s2) const noexcept
		{
			return false;
		}
	};
	template<>
	struct less<cyng::custom>
	{
		using result_type = bool;
		using first_argument_type = cyng::custom;
		using second_argument_type = cyng::custom;

		inline bool operator()(cyng::custom const& s1, cyng::custom const& s2) const noexcept
		{
			return false;
		}
	};
}

namespace cyng
{

	bool test_io_002()
	{
		
		const auto tmp = boost::filesystem::temp_directory_path() / "test_io_002.bin";
		const std::string file_name(tmp.string());
		{
			std::cout << file_name << std::endl;
			std::fstream f(file_name, std::ios::binary | std::ios::trunc | std::ios::out);
			//BOOST_ASSERT_MSG(f.is_open(), "bad state");
			BOOST_CHECK(f.is_open());

			io::serialize_binary(f, make_object());	//	test NULL value

			io::serialize_binary(f, make_object(true));	
			io::serialize_binary(f, make_object(static_cast<char>(33)));	//	ASCII 33 == !, serialized as string
			io::serialize_binary(f, make_object(boost::math::constants::e<float>()));	//	2.71828
			io::serialize_binary(f, make_object(boost::math::constants::pi<double>()));	//	3.14159
			//	differebt sizes on different compilers
			//io::serialize_binary(f, make_object(boost::math::constants::phi<long double>()));	//	1.61803

			io::serialize_binary(f, make_object(static_cast<std::uint8_t>(34)));
			io::serialize_binary(f, make_object(static_cast<std::uint16_t>(3)));
			io::serialize_binary(f, make_object(static_cast<std::uint32_t>(4)));
			io::serialize_binary(f, make_object(static_cast<std::uint64_t>(5)));

			io::serialize_binary(f, make_object(static_cast<std::int8_t>(35)));
			io::serialize_binary(f, make_object(static_cast<std::int16_t>(7)));
			io::serialize_binary(f, make_object(static_cast<std::int32_t>(8)));
			io::serialize_binary(f, make_object(static_cast<std::int64_t>(9)));

			io::serialize_binary(f, make_object(""));
			io::serialize_binary(f, make_object("hello, world!"));

			io::serialize_binary(f, make_object(chrono::init_tp(2018, 4, 20, 6, 5, 32)));
			io::serialize_binary(f, make_object(std::chrono::nanoseconds(10)));
			io::serialize_binary(f, make_object(std::chrono::microseconds(11)));
			io::serialize_binary(f, make_object(std::chrono::milliseconds(12)));
			io::serialize_binary(f, make_object(std::chrono::seconds(13)));
			io::serialize_binary(f, make_object(std::chrono::minutes(14)));
			io::serialize_binary(f, make_object(std::chrono::hours(15)));
			io::serialize_binary(f, make_object(cyng::chrono::to_dbl_time_point(chrono::init_tp(2018, 4, 20, 6, 5, 33))));
			//io::serialize_binary(f, make_object(cyng::chrono::convert(std::chrono::system_clock::now())));

			io::serialize_binary(f, make_object(cyng::version(1, 5)));
			io::serialize_binary(f, make_object(cyng::revision(cyng::version(1, 6), cyng::version(7, 8))));

			//code,
            io::serialize_binary(f, make_object(cyng::code::ESBA));
			//label,
			//logging::severity,
			io::serialize_binary(f, make_object(cyng::logging::severity::LEVEL_WARNING));
			//buffer_t,
			io::serialize_binary(f, make_object(cyng::buffer_t{16, 17, 18}));
			//mac48,
			io::serialize_binary(f, make_object(cyng::mac48(0x00, 0xFF, 0xB0, 0x4B, 0xBE, 0xAA)));
			//mac64,
			io::serialize_binary(f, make_object(cyng::mac64(0x00, 0xFF, 0xB0, 0x4B, 0xBE, 0xAA, 0xBB, 0xCC)));
			//color_8,
			//color_16,
		
			//crypto::digest_md5,
			io::serialize_binary(f, make_object(crypto::digest_md5(md5_hash("hash me"))));
			//crypto::digest_sha1,
			io::serialize_binary(f, make_object(crypto::digest_sha1(sha1_hash("hash me"))));
			//crypto::digest_sha256,
			io::serialize_binary(f, make_object(crypto::digest_sha256(sha256_hash("hash me"))));
			//crypto::digest_sha512,
			io::serialize_binary(f, make_object(crypto::digest_sha512(sha512_hash("hash me"))));

			//object, 		//	embedded object - don't do this
		
			//tuple_t,
			//vector_t,
			io::serialize_binary(f, make_object(vector_factory({ 1, 2, 3 })));
			io::serialize_binary(f, make_vector());	//	empty vector
			//set_t,

			//attr_map_t,		//	std::map<std::size_t, object>;
			//attr_t,			//	std::pair<std::size_t, object>;
			//param_map_t,	//	std::map<std::string, object>;
			//param_t,		//	std::pair<std::string, object>;
		
			//lockable,
		
			//boost::system::error_code,
			//boost::uuids::uuid,
			io::serialize_binary(f, make_object(boost::uuids::string_generator()("2f28413a-d69f-4fc6-b39b-14ff401b15d2")));
			//boost::filesystem::path,
			io::serialize_binary(f, make_object(boost::filesystem::path("demo.txt")));
			//boost::asio::ip::tcp::endpoint,
			io::serialize_binary(f, make_object(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 20015)));
			//boost::asio::ip::udp::endpoint,
			io::serialize_binary(f, make_object(boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 20016)));
			//boost::asio::ip::icmp::endpoint,
			io::serialize_binary(f, make_object(boost::asio::ip::icmp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 20017)));
			//boost::asio::ip::address,
			io::serialize_binary(f, make_object(boost::asio::ip::address::from_string("172.16.254.1")));

			//io::serialize_binary(f, make_object(0xAA55AA55)); // not portable!

			//
			//	dealing with large values
			//
#ifdef _TEST_LARGE_TYPES
			cyng::random_string_factory rsf("abcdefghijklmnopqrstuvwxyz");
 			io::serialize_binary(f, rsf(1000));
#endif

			//
			//	custom objects
			//
#ifdef _TEST_CUSTOM_TYPES
			custom c;
			io::serialize_binary(f, make_object(c));
#endif
		}
		{
			std::fstream f(file_name, std::ios::binary | std::ios::in);
			BOOST_CHECK(f.is_open());
			//	otherwise all whitespaces get lost
			f >> std::noskipws;	//	same as  f.unsetf(std::ios::skipws);
		
			parser np([](vector_t&& prg){
				//	empty callback
				//std::cout << prg.size() << std::endl;
				BOOST_CHECK_EQUAL(prg.size(), 47);
				std::size_t index{ 0 };
				for (auto obj : prg)
				{
					//io::serialize_plain(std::cout, obj);
					//std::cout << " ";
					auto const str = io::to_str(obj);

					switch (index) {
					case 0:
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_NULL);
						BOOST_CHECK_EQUAL(str, "null");
						break;
					case 1:
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_BOOL);
						BOOST_CHECK_EQUAL(str, "true");
						break;
					case 2:
						//	get string (15)! - serialized as string
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_STRING);
						BOOST_CHECK_EQUAL(str, "!");
						break;
					case 3:
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_FLOAT);
						BOOST_CHECK_EQUAL(str, "2.71828");
						break;
					case 4: 
						//d 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_DOUBLE);
						BOOST_CHECK_EQUAL(str, "3.14159");
						break;
					case 5:
						//u8 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_UINT8);
						BOOST_CHECK_EQUAL(str, "22");
						break;
					case 6:
						//u16 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_UINT16);
						BOOST_CHECK_EQUAL(str, "0003");
						break;
					case 7:
						//u32 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_UINT32);
						BOOST_CHECK_EQUAL(str, "00000004");
						break;
					case 8:
						//u64 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_UINT64);
						BOOST_CHECK_EQUAL(str, "5");
						break;
					case 9:
						//i8 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_INT8);
						BOOST_CHECK_EQUAL(str, "35");
						break;
					case 10:
						//i16 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_INT16);
						BOOST_CHECK_EQUAL(str, "7");
						break;
					case 11:
						//i32 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_INT32);
						BOOST_CHECK_EQUAL(str, "8");
						break;
					case 12:
						//i64 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_INT64);
						BOOST_CHECK_EQUAL(str, "9");
						break;
					case 13:
						//s
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_STRING);
						break;
					case 14: 
						//s 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_STRING);
						BOOST_CHECK_EQUAL(str, "hello, world!");
						break;
					case 15:
						//	chrono:tp 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_TIME_POINT);
						BOOST_CHECK_EQUAL(str, "2018-04-20 06:05:32.00000000");
						break;
					case 16:
						//chrono:ns 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_NANO_SECOND);
						BOOST_CHECK_EQUAL(str, "00:00:0.000000");
						break;
					case 17:
						//chrono:us 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_MICRO_SECOND);
						BOOST_CHECK_EQUAL(str, "00:00:0.000011");
						break;
					case 18:
						//chrono:ms 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_MILLI_SECOND);
						BOOST_CHECK_EQUAL(str, "00:00:0.012000");
						break;
					case 19:
						//chrono:sec 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_SECOND);
						BOOST_CHECK_EQUAL(str, "00:00:13.000000");
						break;
					case 20:
						//chrono:min 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_MINUTE);
						BOOST_CHECK_EQUAL(str, "00:14:0.000000");
						break;
					case 21:
						//chrono:h 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_HOUR);
						BOOST_CHECK_EQUAL(str, "15:00:0.000000");
						break;
					case 22:
						//chrono:dtp 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_DBL_TP);
						BOOST_CHECK_EQUAL(str, "2018-04-20 06:05:33.00000000");
						break;
					case 23:
						//v 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_VERSION);
						BOOST_CHECK_EQUAL(str, "1.5");
						break;
					case 24:
						//rev 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_REVISION);
						BOOST_CHECK_EQUAL(str, "1.6.7.8");
						break;
					case 25:
						//op 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_CODE);
						BOOST_CHECK_EQUAL(str, "op:ESBA");
						break;
					case 26:
						//log:severity 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_SEVERITY);
						BOOST_CHECK_EQUAL(str, "WARN ");
						break;
					case 27:
						//binary 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_BUFFER);
						BOOST_CHECK_EQUAL(str, "101112");
						break;
					case 28:
						//mac48 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_MAC48);
						BOOST_CHECK_EQUAL(str, "00:ff:b0:4b:be:aa");
						break;
					case 29:
						//mac64 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_MAC64);
						BOOST_CHECK_EQUAL(str, "ff00:4bb0:aabe:ccbb");
						break;
					case 30:
						//crypto:MD5 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_DIGEST_MD5);
						BOOST_CHECK_EQUAL(str, "17b31dce96b9d6c6d0a6ba95f47796fb");
						break;
					case 31:
						//crypto:SHA1 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_DIGEST_SHA1);
						BOOST_CHECK_EQUAL(str, "43f932e4f7c6ecd136a695b7008694bb69d517bd");
						break;
					case 32:
						//crypto:SHA256 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_DIGEST_SHA256);
						BOOST_CHECK_EQUAL(str, "eb201af5aaf0d60629d3d2a61e466cfc0fedb517add831ecac5235e1daa963d6");
						break;
					case 33:
						//crypto:SHA512 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_DIGEST_SHA512);
						BOOST_CHECK_EQUAL(str, "8529afcbc87cc6ea6eac37d12b60d9a87095170811b6b61036fda72a5e5446041b29ca48fc20aa3a493f62fd10b55e6ccaca4ce18e7bd7e285ec30929783bf59");
						break;
					case 34:
						//i32 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_INT32);
						BOOST_CHECK_EQUAL(str, "1");
						break;
					case 35:
						//i32 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_INT32);
						BOOST_CHECK_EQUAL(str, "2");
						break;
					case 36:
						//i32 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_INT32);
						BOOST_CHECK_EQUAL(str, "3");
						break;
					case 37:
						//u64 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_UINT64);
						BOOST_CHECK_EQUAL(str, "3");
						break;
					case 38:
						//op 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_CODE);
						BOOST_CHECK_EQUAL(str, "op:VEC");
						break;
					case 39:
						//u64 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_UINT64);
						BOOST_CHECK_EQUAL(str, "0");
						break;
					case 40:
						//op 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_CODE);
						BOOST_CHECK_EQUAL(str, "op:VEC");
						break;
					case 41:
						//uuid 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_UUID);
						BOOST_CHECK_EQUAL(str, "2f28413a-d69f-4fc6-b39b-14ff401b15d2");
						break;
					case 42:
						//fs:path 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_FS_PATH);
						BOOST_CHECK_EQUAL(str, "\"demo.txt\"");
						break;
					case 43:
						//ip:tcp:endpoint 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_IP_TCP_ENDPOINT);
						BOOST_CHECK_EQUAL(str, "127.0.0.1:20015");
						break;
					case 44:
						//ip:udp:endpoint 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_IP_UDP_ENDPOINT);
						BOOST_CHECK_EQUAL(str, "127.0.0.1:20016");
						break;
					case 45:
						//ip:icmp:endpoint 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_IP_ICMP_ENDPOINT);
						BOOST_CHECK_EQUAL(str, "127.0.0.1:20017");
						break;
					case 46:
						//ip:address 
						BOOST_CHECK_EQUAL(obj.get_class().tag(), TC_IP_ADDRESS);
						BOOST_CHECK_EQUAL(str, "172.16.254.1");
						break;

					default:
						std::cout
							<< index
							<< " "
							<< obj.get_class().type_name()
							<< " "
							<< str
							<< std::endl;
						break;
					}
					++index;
				}
			});
			np.read(std::istream_iterator<char>(f), std::istream_iterator<char>());			
			std::cout << std::endl;
		}
		return true;
	}
	
}
