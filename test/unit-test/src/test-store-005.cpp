
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 
#include "test-store-005.h"
#include <iostream>
#include <cyng/table/body.hpp>
#include <cyng/table/meta.hpp>
#include <cyng/store/db.h>
#include <cyng/value_cast.hpp>
#include <cyng/intrinsics.h>
//#include <cyng/crypto/aes.h>

#include <boost/test/unit_test.hpp>
#include <boost/uuid/nil_generator.hpp>

namespace cyng 
{

	
	bool test_store_005()
	{
		//
		//	test storage of "exotic" data types
		//
		store::db database;
		BOOST_CHECK(database.create_table(cyng::table::make_meta_table<1, 2>("tbl.05",
			{ "pk", "device", "key" },
			{ TC_UINT32, TC_STRING, TC_AES128 },
			{ 0, 128, 0 }), false));

		crypto::aes_128_key key;
		for (std::uint32_t idx = 0u; idx < 100u; ++idx)
		{
			//
			//	new key
			//
			//crypto::aes::randomize(key);

			//database.insert("tbl.05"
			//	, table::key_generator(idx)
			//	, table::data_generator(std::to_string(idx)
			//	, key)
			//	, 0	//	generation
			//	, boost::uuids::nil_uuid());
		}

		database.access([](store::table const* tbl)->void {

			crypto::aes_128_key key;
			for (std::uint32_t idx = 0u; idx < 100u; ++idx) {
				auto rec = tbl->lookup(table::key_generator(idx));
				BOOST_CHECK(!rec.empty());

				key = cyng::value_cast(rec["key"], key);
				auto const pk = cyng::value_cast<std::uint32_t>(rec["pk"], 0u);
				BOOST_CHECK_EQUAL(pk, idx);
			}

		}, store::read_access("tbl.05"));

		return true;
	}
	
}
