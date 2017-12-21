/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_ASYNC_MUX_H
#define CYNG_ASYNC_MUX_H

#include <cyng/async/scheduler.h>
#include <cyng/async/task_fwd.h>
#include <cyng/intrinsics/sets.h>
#include <cyng/compatibility/io_service.h>

namespace cyng 
{
	using tuple_t = std::list<object>;
	namespace async 
	{
		/** @brief message multiplexer for tasks
		 * 
		 * 	There are three options to start a task:
		 * 
		 * (1) sync - insert task into task list and call run() method
		 * in the same thread.
		 * 
		 * (2) async - insert task into task list and call run() method
		 * in another thread.
		 * 
		 * (3) delayed - insert task into task list and call run() method
		 * from a timer callback.
		 */
		class mux
		{
			friend class base_task;
			template < typename T, typename ...Args >
			friend bool start_task_sync(mux& m, Args &&... args);
			template < typename T, typename ...Args >
			friend void start_task_detached(mux& m, Args &&... args);
			template < typename T, typename R, typename P, typename ...Args >
			friend bool start_task_delayed(mux& m, std::chrono::duration<R, P> d, Args &&... args);
			
		public:
			/**
			 *	constructor
			 */
			mux();
			mux(unsigned int);
			virtual ~mux();
			
			/**
			 * @return count of running tasks
			 */
			std::size_t size() const;

			/**
			 * @return true if task exists
			 */
			bool is_task(std::size_t id) const;
			
			/**
			 * This function may be used to obtain the io_service object 
			 * that the scheduler uses to dispatch handlers for asynchronous operations. 
			 */
			inline io_service_t& get_io_service()
			{
				return scheduler_.get_io_service();
			}
			
			/**
			 * Gracefull shutdown of the internal scheduler.
			 * @return true if shutdown was successfull
			 */
			void shutdown();
			
			/**
			 * Terminate all tasks. Doesn't stop the scheduler.
			 * @return true if stop was successfull
			 */
			bool stop();
			
			/**
			 * Stop a specific task and remove it from task list. 
			 * Works asynchronously.
			 *
			 * @return true if request could be internally queued.
			 * It's no guaranty that specified task exists and would be stopped.
			 */
			bool stop(std::size_t);
			
			/**
			 * Works asynchronously.
			 *
			 * @return true if message could be delivered
			 */
			bool send(std::size_t id, std::size_t slot, tuple_t&& tpl) const;
			
		private:
			/**
			 * Non-blocking function.
			 *
			 * @return next unique task tag 
			 */
			std::size_t next_id();
			
			/**
			 * @return true if insert was successful
			 */
			bool insert(shared_task, sync);
			
			/**
			 * Insert task into task list but don't wait.
			 */
			void insert(shared_task, detach);
			
			/**
			 * Insert task into task but don't call run()
			 */
			bool insert(shared_task, none);

			
		private:
			/**
			* distribute all tasks to the
			* thread pool
			*/
			mutable	scheduler	scheduler_;
			mutable dispatcher_t dispatcher_;
			
			/**
			 * Each task is identified by a unique
			 * tag. The tag is created by incremeting 
			 * this value
			 */
			std::atomic< std::size_t >	next_tag_;
			
			/**
			 * The task list is a container that maps
			 * a task tag to a shared task pointer
			 */ 
			task_map	tasks_;
			
			/**
			 * The shutdown flag is required to detect a situation
			 * when a (locked) task tries to call the stop() method
			 * during shutdown. Otherwise this would create a deadlock.
			 */
			std::atomic< bool >	shutdown_;
			
		};

	}	// async
}

#endif	//	CYNG_ASYNC_MUX_H


