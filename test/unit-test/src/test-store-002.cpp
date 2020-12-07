
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#include "test-store-002.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/table/body.hpp>
#include <cyng/table/meta.hpp>
#include <cyng/store/db.h>
#include <cyng/factory.h>
#include <chrono>
#include <future>
#include <atomic>
#include <boost/uuid/nil_generator.hpp>

namespace cyng 
{
	static std::atomic<int> access_counter{ 0 };

	void fun_1(store::db& v)
	{
		v.access([](store::table* t1, const store::table* t2)->void{
			
			++access_counter;
			BOOST_CHECK_EQUAL(access_counter, 1);
			--access_counter;
			//std::cout 
			//<< "2a. attempt: "
			//<< t1->meta().get_name() 
			//<< ", "
			//<< t2->meta().get_name() 
			//<< std::endl;
			
		}, store::write_access("table-1"), store::read_access("table-2"));
		
// 		std::this_thread::yield();
// 		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	void fun_2(store::db& v)
	{
		v.access([](const store::table* t1, store::table* t2)->void{
			
			++access_counter;
			BOOST_CHECK_EQUAL(access_counter, 1);
			--access_counter;
			//std::cout 
			//<< "2b. attempt: "
			//<< t1->meta().get_name() 
			//<< ", "
			//<< t2->meta().get_name() 
			//<< std::endl;
			
		}, store::read_access("table-1"), store::write_access("table-2"));
		
// 		std::this_thread::yield();
// 		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	
	bool test_store_002()
	{
		//
		//	test concurrent table access
		//
		auto dbmtp = table::make_meta_table<1, 2>("db", {"name", "table", "created"});
		store::table db_tbl(dbmtp, false);
		
		auto mtp = table::make_meta_table<2, 5>("table-2", {"key-0", "key-1", "body-0", "body-1", "body-2", "body-3", "body-4"});
		auto tbl_obj = make_object<store::table>(mtp, false);
		
// 		key_type const& key, data_type const& data
		const bool b = db_tbl.insert(table::key_generator("name"), table::data_generator(tbl_obj, std::chrono::system_clock::now()), 0, boost::uuids::nil_uuid());
		BOOST_CHECK(b);
		
		store::db db_1;
		db_1.create_table(table::make_meta_table<2, 5>("table-1", {"key-0", "key-1", "body-0", "body-1", "body-2", "body-3", "body-4"}), false);
		db_1.create_table(table::make_meta_table<2, 5>("table-2", {"key-0", "key-1", "body-0", "body-1", "body-2", "body-3", "body-4"}), false);
		
		db_1.access([](store::table const* t1, store::table* t2)->void{
			
			//std::cout 
			//<< "1. attempt: "
			//<< t1->meta().get_name() 
			//<< ", "
			//<< t2->meta().get_name() 
			//<< std::endl;
			
// 			std::this_thread::sleep_for(std::chrono::seconds(5));
			
		}, store::read_access("table-1"), store::write_access("table-2"));
		
		for (std::size_t idx = 0; idx < 100; idx++)
		{
			std::thread t1(fun_1, std::ref(db_1));
			std::thread t2(fun_2, std::ref(db_1));
			t1.detach();
			t2.detach();
		}
		
		
		db_1.access([](store::table* t1, const store::table* t2)->void{
			
			//std::cout 
			//<< "3. attempt: "
			//<< t1->meta().get_name() 
			//<< ", "
			//<< t2->meta().get_name() 
			//<< std::endl;
			
		}, store::write_access("table-1"), store::read_access("table-2"));

		std::this_thread::sleep_for(std::chrono::seconds(5));
		return access_counter == 0;
	}
	
}
