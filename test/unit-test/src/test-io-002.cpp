
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
			using tag = std::integral_constant<std::size_t, PREDEF_CUSTOM>;
#if defined(CYNG_LEGACY_MODE_ON)
			const static char name[];
#else
			constexpr static char name[] = "custom";
#endif
		};

#if defined(CYNG_LEGACY_MODE_ON)
		const char type_tag<custom>::name[] = "custom";
#endif

		template <>
		struct reverse_type < PREDEF_CUSTOM >
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
			BOOST_ASSERT_MSG(f.is_open(), "bad state");

			io::serialize_binary(f, make_object());	//	test NULL value

			io::serialize_binary(f, make_object(true));	
			io::serialize_binary(f, make_object(static_cast<char>(33)));
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
			io::serialize_binary(f, make_object(cyng::ESBA));
			//label,
			//logging::severity,
			io::serialize_binary(f, make_object(cyng::logging::severity::LEVEL_WARNING));
			//buffer_t,
			io::serialize_binary(f, make_object(cyng::buffer_t{16, 17, 18}));
			//mac48,
			io::serialize_binary(f, make_object(cyng::mac48(0x00, 0xFF, 0xB0, 0x4B, 0xBE, 0xAA)));
			//mac64,
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
			BOOST_ASSERT_MSG(f.is_open(), "bad state");
			//	otherwise all whitespaces get lost
			f >> std::noskipws;	//	same as  f.unsetf(std::ios::skipws);
		
			parser np([](vector_t&& prg){
				//	empty callback
				std::cout << prg.size() << std::endl;
				for (auto obj : prg)
				{
					io::serialize_plain(std::cout, obj);
					std::cout << " ";
				}
			});
			np.read(std::istream_iterator<char>(f), std::istream_iterator<char>());			
			std::cout << std::endl;
		}
		return true;
	}
	
}
