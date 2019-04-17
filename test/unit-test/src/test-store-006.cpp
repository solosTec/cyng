
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 
#include "test-store-006.h"
#include <iostream>
#include <cyng/table/body.hpp>
#include <cyng/table/meta.hpp>
#include <cyng/store/db.h>
#include <cyng/value_cast.hpp>
#include <cyng/intrinsics.h>
#include <cyng/io/serializer.h>

#include <boost/test/unit_test.hpp>
#include <boost/uuid/nil_generator.hpp>
#include <boost/uuid/random_generator.hpp>

namespace cyng 
{
	bool test_store_006()
	{
		//
		//	test merge() function
		//
		store::db database;
		BOOST_CHECK(database.create_table(cyng::table::make_meta_table<1, 2>("tbl.06",
			{ "id", "name", "ts" },
			{ TC_UINT32, TC_STRING, TC_TIME_POINT },
			{ 0, 128, 0 })));

		//
		//	random sources
		//
		boost::uuids::random_generator uuid_gen;

		//
		//	test marker
		//
		std::size_t m_inserts{ 0 };
		std::size_t m_updates{ 0 };

		//
		//	modify listener
		//
		database.get_modify_listener("tbl.06", [&](store::table const* tbl
			, table::key_type const& key
			, attr_t const& attr
			, std::uint64_t gen
			, boost::uuids::uuid source) {

				++m_updates;
				//std::cout << "modify: " << io::to_str(key) << ": " << io::to_str(attr.second) << std::endl;
		});

		//
		//	insert listener
		//
		database.get_insert_listener("tbl.06", [&](store::table const* tbl
			, cyng::table::key_type const& key
			, cyng::table::data_type const& data
			, std::uint64_t gen
			, boost::uuids::uuid source) {

				++m_inserts;
				//std::cout << "insert: " << io::to_str(key) << ": " << io::to_str(data) << std::endl;
			});

		//
		//	insert [0..20] records
		//
		for (std::uint32_t idx = 0u; idx < 20; ++idx)
		{
			database.insert("tbl.06"
				, table::key_generator(idx)
				, table::data_generator(std::to_string(idx), std::chrono::system_clock::now())
				, 0	//	generation
				, uuid_gen());
		}

		//
		//	merge [10..30] records
		//
		database.access([&](store::table* tbl)->void {

			for (std::uint32_t idx = 10u; idx < 30; ++idx) {

				tbl->merge(table::key_generator(idx),
					table::data_generator(std::to_string(idx), std::chrono::system_clock::now()),
					0, uuid_gen());
			}

		}, store::write_access("tbl.06"));

		BOOST_CHECK_EQUAL(m_inserts, 30);
		BOOST_CHECK_EQUAL(m_updates, 10);

		return true;
	}
	
}
