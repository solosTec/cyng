/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SCRIPT_TASK_SCHEDULER_H
#define CYNG_SCRIPT_TASK_SCHEDULER_H

#include <boost/asio.hpp>
#include <cyng/obj/intrinsics/container.h>
#include <cyng/task/channel.h>

#include <thread>

namespace cyng {

	class scheduler
	{
	public:
		using io_context_t = boost::asio::io_context;
		using dispatcher_t = boost::asio::io_context::strand;

	public:
		scheduler();
		scheduler(std::size_t num_threads);
		void stop();
		io_context_t& get_ctx();

		/**
		 *  Invoke the given handler and return immediately.
		 */
		template < typename H >
		void post(H&& h)
		{
			boost::asio::post(ctx_, std::forward<H>(h));
		}

	private:
		void start(std::size_t num_threads);
		void join();

	protected:
		io_context_t	ctx_;

	private:
		boost::asio::executor_work_guard<io_context_t::executor_type>	work_;
		std::vector< std::thread >	pool_;
	};

}

#endif
