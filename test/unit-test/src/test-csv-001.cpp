
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 
#include "test-csv-001.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/csv.h>
#include <cyng/object.h>
#include <cyng/factory.h>
#include <cyng/io/serializer.h>
#include <boost/uuid/random_generator.hpp>
#include <boost/filesystem.hpp>

namespace cyng 
{

	bool test_csv_001()
	{
		boost::uuids::random_generator rgen;

		//
		//	create a vector of tuples
		//
		const auto vec = cyng::vector_factory({
			cyng::tuple_factory(rgen(), "name-1", 1),
			cyng::tuple_factory(rgen(), "name-2", 2),
			cyng::tuple_factory(rgen(), "name-3", 3),
			cyng::tuple_factory(rgen(), "name-4", 4),
			cyng::tuple_factory(rgen(), "name-5", 5)
		});
		//std::cout << io::to_str(vec) << std::endl;
		auto const s1 = io::to_str(vec);


		//
		//	serialize to CSV
		//
		auto const tmp = boost::filesystem::temp_directory_path() / boost::filesystem::unique_path("unit-test-%%%%-%%%%-%%%%-%%%%.csv");
		const std::string file_name(tmp.string());
		{
			//std::cout << file_name << std::endl;
			std::ofstream f(file_name, std::ios::binary | std::ios::trunc | std::ios::out);
			BOOST_CHECK(f.is_open());
			csv::write(f, make_object(vec));
		}

		//
		//	deserialize from CSV
		//
		{
			auto const vec = csv::read_file(tmp.string());
			//std::cout << io::to_str(r) << std::endl;
			auto const s2 = io::to_str(vec);

			//
			//	serialized and parsed data should be equal
			//
			BOOST_CHECK_EQUAL(s1, s2);
		}

		boost::system::error_code ec;
		boost::filesystem::remove(tmp, ec);
		BOOST_CHECK_MESSAGE(!ec, "cannot delete file");

		return true;
	}
	
}
