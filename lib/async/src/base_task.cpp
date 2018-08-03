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
		
		std::size_t base_task::cancel_timer()
		{
			return timer_.cancel();
		}

		void base_task::remove_this()
		{
			mux_.remove(id_);
		}

		void base_task::suspend_until(std::chrono::system_clock::time_point tp)
		{
			if (shutdown_)	return;

			auto ptr = get_shared();
			timer_.expires_from_now(tp - std::chrono::system_clock::now());
			timer_.async_wait([ptr](boost::system::error_code const& ec) {
				if (ec != boost::asio::error::operation_aborted)
				{
					ptr->run();
				}
			});

		}


	}	// async
}







