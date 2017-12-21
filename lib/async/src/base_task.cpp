/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 


#include <cyng/async/task/base_task.h>
#include <cyng/async/mux.h>

namespace cyng 
{
	namespace async 
	{
		base_task::base_task(mux& m)
		: mux_(m)
		, id_(mux_.next_id())
		, timer_(m.get_io_service())
		, dispatcher_(m.get_io_service())
		, shutdown_(false)
		{}
			
		std::size_t base_task::get_id() const
		{
			return id_;
		}
		
		void base_task::cancel_timer()
		{
			timer_.cancel();
		}

	}	// async
}







