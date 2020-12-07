
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#include "test-store-003.h"
#include <iostream>
#include <cyng/table/body.hpp>
#include <cyng/table/meta.hpp>
#include <cyng/store/db.h>
#include <cyng/factory.h>
#include <cyng/value_cast.hpp>
#include <chrono>
#include <future>
#include <boost/test/unit_test.hpp>
#include <boost/uuid/nil_generator.hpp>

namespace cyng 
{

	
	bool test_store_003()
	{
		//
		//	test multi_modify()
		//
		store::db database;
		BOOST_CHECK(database.create_table(cyng::table::make_meta_table<1, 3>("table-1",
			{ "pk", "field-1", "field-2", "field-2" },
			{ cyng::TC_INT32, cyng::TC_STRING, cyng::TC_STRING, cyng::TC_STRING },
			{ 0, 128, 128, 128 }), false));
		
		BOOST_CHECK(database.create_table(cyng::table::make_meta_table<1, 3>("table-2",
			{ "pk", "field-1", "field-2", "field-2" },
			{ cyng::TC_INT32, cyng::TC_STRING, cyng::TC_STRING, cyng::TC_STRING },
			{ 0, 128, 128, 128 }), false));

		BOOST_CHECK(database.insert("table-1"
			, table::key_generator(1)
			, table::data_generator("a", "a", "a")
			, 1	//	generation
			, boost::uuids::nil_uuid()));
		BOOST_CHECK(database.insert("table-1"
			, table::key_generator(2)
			, table::data_generator("b", "b", "b")
			, 1	//	generation
			, boost::uuids::nil_uuid()));

		//	std::string const& name, cyng::table::key_type const& key, param_t const& param, boost::uuids::uuid source
		BOOST_CHECK(database.multi_modify("table-1"
			, table::key_generator(1)
			, param_factory("field-2", "new")
			, boost::uuids::nil_uuid()
			, [&](cyng::table::key_type const& key, param_t const& param, boost::uuids::uuid source, store::table* t1, const store::table* t2)->bool {

			//
			//	manipulate table t2 at the same time
			//

			//std::cout << t2->meta().get_name() << std::endl;
			return t1->modify(key, param, source);
		}, store::read_access("table-2")));

		database.access([](store::table const* t1, store::table const* t2)->void {
			auto rec = t1->lookup(table::key_generator(1));
			BOOST_CHECK_EQUAL(value_cast<std::string>(rec["field-2"], ""), "new");
		}, store::read_access("table-1"), store::read_access("table-2"));
		return true;
	}
	
}
