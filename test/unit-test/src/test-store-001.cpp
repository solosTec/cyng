
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#include "test-store-001.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/table/body.hpp>
#include <cyng/table/meta.hpp>
#include <cyng/store/table.h>
#include <boost/uuid/nil_generator.hpp>

namespace cyng 
{

	bool test_store_001()
	{
		auto body = table::body_generator(1, 2, 3);
		
		
		table::meta_table<2, 5> mt1("table-1");
		table::meta_table<2, 5> mt2("table-2", {"key-0", "key-1", "body-0", "body-1", "body-2", "body-3", "body-4"});
		
// 		std::cout << mt2.get_body_index("body-3").first << std::endl;
		BOOST_CHECK_EQUAL(mt2.get_body_index("body-3").first, 3);
		
		auto mtp = table::make_meta_table<2, 5>("table-2", {"key-0", "key-1", "body-0", "body-1", "body-2", "body-3", "body-4"}, {});
// 		std::cout << mtp->get_body_index("body-4").first << std::endl;
		BOOST_CHECK_EQUAL(mt2.get_body_index("body-4").first, 4);
		
		store::table tbl_1(mtp);
		tbl_1.insert(table::key_generator(1, 2), table::data_generator(1, 2, 3, 4, 5), 0, boost::uuids::nil_uuid());
		//tbl_1.insert(store::key_generator(1, 2), store::data_generator(1, 2, 3, 4, 5));

// 		std::cout << tbl_1.size() << std::endl;
		BOOST_CHECK_EQUAL(tbl_1.size(), 1);
		
		tbl_1.erase(table::key_generator(1, 2), boost::uuids::nil_uuid());
//  		std::cout << tbl_1.size() << std::endl;
		BOOST_CHECK_EQUAL(tbl_1.size(), 0);
		
// 		constexpr int upper_limit = 0x1000;
		constexpr int upper_limit = 0x100;
// 		constexpr int upper_limit = 0xF;
		
		for (int i1 = 0; i1 < upper_limit; i1++)
		{
// 			std::cout << i1 << ", " << tbl_1.size() << std::endl;
			for (int i2 = 0; i2 < upper_limit; i2++)
			{
//  				std::cout << "INS: " << i1 << ", " << i2 << ", " << tbl_1.size() << ", " << (i1*upper_limit +i2) << std::endl;
 				BOOST_CHECK_EQUAL(tbl_1.size(), (i1*upper_limit + i2));
				tbl_1.insert(table::key_generator(i1, i2), table::data_generator(i1, i2, i1+i2, i1-i2, i2-i1), 0, boost::uuids::nil_uuid());
			}
		}
		
		for (int i1 = 0; i1 < upper_limit; i1++)
		{
//  			std::cout << i1 << ", " << tbl_1.size() << std::endl;
			for (int i2 = 0; i2 < upper_limit; i2++)
			{
// 				std::cout << i1 << ", " << i2 << ", " << tbl_1.size() << ", " << (i1*upper_limit +i2) << std::endl;
//  				BOOST_CHECK_EQUAL(tbl_1.size(), (i1*upper_limit +i2));
// 				tbl_1.insert(store::key_generator(i1, i2), store::data_generator(i1, i2, i1+i2, i1-i2, i2-i1));
				auto rec = tbl_1.lookup(table::key_generator(i1, i2));
				BOOST_CHECK(!rec.empty());
				
				BOOST_CHECK_EQUAL(value_cast(rec["body-0"], 0), i1);
				BOOST_CHECK_EQUAL(value_cast(rec["body-1"], 0), i2);
				BOOST_CHECK_EQUAL(value_cast(rec["body-2"], 0), i1+i2);
				BOOST_CHECK_EQUAL(value_cast(rec["body-3"], 0), i1-i2);
				BOOST_CHECK_EQUAL(value_cast(rec["body-4"], 0), i2-i1);
// 				std::cout << "OUT: "  
				
// 				<< value_cast(rec[2], 0) << ", "
// 				<< value_cast(rec[3], 0) << ", "
// 				<< value_cast(rec[4], 0) << ", "
// 				<< value_cast(rec[5], 0) << ", "
// 				<< value_cast(rec[6], 0)
				
// 				<< value_cast(rec["body-0"], 0) << ", "
// 				<< value_cast(rec["body-1"], 0) << ", "
// 				<< value_cast(rec["body-2"], 0) << ", "
// 				<< value_cast(rec["body-3"], 0) << ", "
// 				<< value_cast(rec["body-4"], 0)
				
// 				<< std::endl;
				
			}
		}

		return true;
	}
	
}
