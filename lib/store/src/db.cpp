/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/store/db.h>
#include <cyng/store/meta.hpp>
#include <cyng/intrinsics/traits/tag.hpp>

#include <chrono>

namespace cyng 
{
	namespace store 
	{

		db::db()
		: tables_(make_meta_table<1, 2>("db", {"name", "table", "created"}))
		, m_()
		{}
		
		db::~db()
		{
			tables_.clear();
		}
		
		bool db::create_table(meta_table_ptr ptr)
		{
			//	start with generation 1
			return tables_.insert(key_generator(0, ptr->get_name())
				, data_generator(table(ptr), std::chrono::system_clock::now()), 1);
		}
		
		std::size_t db::size() const
		{
			return tables_.size();
		}	
	}	//	store
	
	namespace traits
	{
	
#if defined(CYNG_LEGACY_MODE_ON)
		const char type_tag<cyng::store::db>::name[] = "db";
#endif
	}	// traits		
}








