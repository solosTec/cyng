
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#include "test-store-004.h"
#include <iostream>
#include <cyng/table/body.hpp>
#include <cyng/table/meta.hpp>
#include <cyng/store/db.h>
#include <cyng/value_cast.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/uuid/nil_generator.hpp>

namespace cyng 
{

	
	bool test_store_004()
	{
		//
		//	test min_record() and max_record() functions
		//

		store::db database;
		BOOST_CHECK(database.create_table(cyng::table::make_meta_table<1, 1>("msg",
			{ "pk", "evt" },
			{ cyng::TC_INT32, cyng::TC_STRING },
			{ 0, 128})));
		
		for (std::uint32_t idx = 0u; idx < 100; ++idx)
		{
			database.insert("msg"
				, table::key_generator(idx)
				, table::data_generator(std::to_string(idx))
				, 0
				, boost::uuids::nil_uuid());
		}

		database.access([](store::table const* tbl)->void {

			auto rec = tbl->min_record();
			BOOST_CHECK(!rec.empty());
			auto const lmin = cyng::value_cast<std::uint32_t>(rec["pk"], -1);
			BOOST_CHECK_EQUAL(lmin, 0);

			rec = tbl->max_record();
			BOOST_CHECK(!rec.empty());
			auto const lmax = cyng::value_cast<std::uint32_t>(rec["pk"], -1);
			BOOST_CHECK_EQUAL(lmax, 99);

		}, store::read_access("msg"));

		return true;
	}
	
}
