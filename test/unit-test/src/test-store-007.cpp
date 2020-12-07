
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 
#include "test-store-007.h"
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
	bool test_store_007()
	{
		//
		//	random sources
		//
		boost::uuids::random_generator uuid_gen;

		//
		//	pre-test
		//
		std::map<object, cyng::table::key_type> index;
		index.emplace(make_object<std::uint32_t>(1), table::key_generator(uuid_gen()));
		index.emplace(make_object<std::uint32_t>(2), table::key_generator(uuid_gen()));

		auto pos = index.find(make_object<std::uint32_t>(1));

		//
		//	test search by index
		//
		store::db database;
		BOOST_CHECK(database.create_table(cyng::table::make_meta_table<1, 3, 1>("tbl.07",
			{ "tag", "id", "name", "ts" },
			{ TC_UUID, TC_UINT32, TC_STRING, TC_TIME_POINT },
			{ 36, 0, 128, 0 }), false));

		//
		//	insert [0..20] records
		//
		for (std::uint32_t idx = 0u; idx < 20u; ++idx)
		{
			database.insert("tbl.07"
				, table::key_generator(uuid_gen())
				, table::data_generator(idx, std::to_string(idx), std::chrono::system_clock::now())
				, 0	//	generation
				, uuid_gen());
		}

		//
		//	find by index
		//
		database.access([](store::table const* tbl)->void {

			BOOST_CHECK_EQUAL(tbl->size(), 20u);
			
			auto const rec = tbl->lookup_by_index(make_object<std::uint32_t>(1));

			for (std::uint32_t idx = 10u; idx < 30; ++idx) {
				auto const rec = tbl->lookup_by_index(make_object(idx));
				if (idx >= 0 && idx < 20) {
					BOOST_CHECK(!rec.empty());
				}
				else {
					BOOST_CHECK(rec.empty());
				}
			}

		}, store::read_access("tbl.07"));

		//
		//	merge [10..30] records
		//
		//database.access([&](store::table* tbl)->void {

		//	for (std::uint32_t idx = 10u; idx < 30; ++idx) {

		//		tbl->merge(table::key_generator(idx),
		//			table::data_generator(std::to_string(idx), std::chrono::system_clock::now()),
		//			0, uuid_gen());
		//	}

		//}, store::write_access("tbl.07"));


		return true;
	}
	
}
