/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/async/pool.h>

namespace cyng 
{
	namespace async 
	{
		pool::pool()
		: workers_()
		//, running_{ ATOMIC_FLAG_INIT }
		{}
		
		pool::~pool()
		{
			join_all();
		}
		
		bool pool::is_running() const
		{
			return !workers_.empty();
		}
		
		std::size_t pool::size() const
		{
			return workers_.size();
		}
		
		void pool::join_all()
		{
			if (is_running())
			{
				for (auto& worker : workers_)
				{
					worker.join();
				}
				
				//	remove all threads from pool
				workers_.clear();
				running_.clear();
			}
		}
	}
}




