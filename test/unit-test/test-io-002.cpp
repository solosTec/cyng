
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
#include <cyng/io/serializer.h>
#include <cyng/io/parser/parser.h>
#include <boost/uuid/string_generator.hpp>

namespace cyng 
{

	bool test_io_002()
	{
		
// 		auto o_null = factory<null>::create_object();
// 		type null_t(o_null.get_class());
// 		std::cout << o_null.get_class().type_name() << std::endl;
// 		io::serialize_plain(std::cout, o_null);
// 		std::cout << std::endl;
// 
//  		auto o_i = make_object(23);
// 		type i_t(o_i.get_class());
// 		std::cout << o_i.get_class().type_name() << std::endl;
// 		io::serialize_plain(std::cout, o_i);
// 		std::cout << ", ";
// 		io::serialize_formatted(std::cout, o_i);
// 		std::cout << std::endl;
// 		
// 		auto o_s = make_object("object");
// 		io::serialize_plain(std::cout, o_s);
// 		io::serialize_json(std::cout, o_s);
// 		std::cout << std::endl;

		const auto tmp = boost::filesystem::temp_directory_path() / "test_io_002.bin";
		const std::string file_name(tmp.string());
		{
			std::cout << file_name << std::endl;
			std::fstream f(file_name, std::ios::binary | std::ios::trunc | std::ios::out);
			BOOST_ASSERT_MSG(f.is_open(), "bad state");

			io::serialize_binary(f, make_object(0xAA55AA55));
			io::serialize_binary(f, make_object("hello, world!"));
// 			io::serialize_binary(f, cyng::set_factory(42, "forty-two"));
 			io::serialize_binary(f, make_object(boost::uuids::string_generator()("2f28413a-d69f-4fc6-b39b-14ff401b15d2")));
			io::serialize_binary(f, make_object("OK"));
// 			io::serialize_binary(f, cyng::param_map_factory());
			io::serialize_binary(f, make_object());	//	test NULL value
// 			io::serialize_binary(f, cyng::buffer_factory("buffer"));
			//io::serialize_binary(f, make_object(3.1415972));

			//
			//	dealing with large values
			//
// 			cyng::random_string_factory rsf("abcdefghijklmnopqrstuvwxyz");
// 			io::serialize_binary(f, rsf(1000));
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
