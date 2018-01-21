/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_ASYNC_TASK_BUILDER_HPP
#define CYNG_ASYNC_TASK_BUILDER_HPP


#include <cyng/async/task/task.hpp>
#include <chrono>

namespace cyng 
{
	namespace async 
	{
		/**
		 * Create a new task class 
		 */
		template < typename T, typename ...Args >
		auto make_task(mux& m, Args &&... args) -> std::shared_ptr<task < T >>
		{
			typedef task < T > task_type;
			return std::make_shared< task_type >(m, std::forward<Args>(args)...);
		}

		/**
		 * 	There are three options to start a task:
		 * (1) sync - insert task into task list and call run() method
		 * in the same thread.
		 * 
		 * (2) async - insert task into task list and call run() method
		 * in another thread.
		 * 
		 * (3) delayed - insert task into task list and call run() method
		 * from a timer callback.
		 */
		template < typename T, typename ...Args >
		std::pair<std::size_t, bool> start_task_sync(mux& m, Args &&... args)
		{
			auto tp = make_task<T>(m, std::forward<Args>(args)...);
			return std::make_pair(tp->get_id(), m.insert(tp, sync()));
		}
		
		template < typename T, typename ...Args >
		std::size_t start_task_detached(mux& m, Args &&... args)
		{
			auto tp = make_task<T>(m, std::forward<Args>(args)...);
			m.insert(tp, detach());
			return tp->get_id();
		}

		template < typename T, typename R, typename P, typename ...Args >
		std::pair<std::size_t, bool> start_task_delayed(mux& m, std::chrono::duration<R, P> d, Args &&... args)
		{
			auto tp = make_task<T>(m, std::forward<Args>(args)...);
			if (m.insert(tp, none()))
			{
				tp->suspend(d);
				return std::make_pair(tp->get_id(), true);
			}
			return std::make_pair(tp->get_id(), false);
		}
		
	}	// async
}

#endif	//	CYNG_ASYNC_TASK_BUILDER_HPP





