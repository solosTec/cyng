
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
//#include <cyng/intrinsics/traits.hpp>
#include <cyng/io/serializer.h>
#include <cyng/io/parser/parser.h>
#include <boost/uuid/string_generator.hpp>

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
			io::serialize_binary(f, make_object(""));
			io::serialize_binary(f, make_object(0xAA55AA55));
			io::serialize_binary(f, make_object("hello, world!"));
 			io::serialize_binary(f, make_object(boost::uuids::string_generator()("2f28413a-d69f-4fc6-b39b-14ff401b15d2")));
			io::serialize_binary(f, make_object("OK"));
// 			io::serialize_binary(f, cyng::buffer_factory("buffer"));
			//io::serialize_binary(f, make_object(3.1415972));

			//
			//	dealing with large values
			//
// 			cyng::random_string_factory rsf("abcdefghijklmnopqrstuvwxyz");
// 			io::serialize_binary(f, rsf(1000));

			//
			//	custom objects
			//
			custom c;
			io::serialize_binary(f, make_object(c));
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
