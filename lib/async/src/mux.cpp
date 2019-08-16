/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/async/mux.h>
#include <cyng/async/task/base_task.h>
#include <boost/algorithm/string/predicate.hpp>
#include <iostream>
#include <algorithm>

namespace cyng 
{
	namespace async 
	{
		mux::mux()
		: scheduler_()
			, dispatcher_(scheduler_.get_io_service())
			, next_tag_(NO_TASK)
			, tasks_()
			, shutdown_(false)
		{
			BOOST_ASSERT_MSG(scheduler_.is_running(), "scheduler not running");
		}
		
		mux::mux(std::size_t count)
		: scheduler_(count)
			, dispatcher_(scheduler_.get_io_service())
			, next_tag_(NO_TASK)
			, tasks_()
			, shutdown_(false)
		{
			BOOST_ASSERT_MSG(scheduler_.is_running(), "scheduler not running");			
		}

		mux::~mux()
		{
			BOOST_ASSERT(tasks_.empty());
		}
		
		bool mux::size(std::function<void(std::size_t)> f) const
		{
			if (shutdown_)	return false;
			BOOST_ASSERT_MSG(scheduler_.is_running(), "scheduler not running");
			
			//
			//	thread safe access to task list
			//
			dispatcher_.dispatch([this, f](){
				f(tasks_.size());
			});
			return true;
		}
		
		bool mux::size(std::string const& name, std::function<void(std::size_t)> f) const
		{
			if (shutdown_)	return false;
			BOOST_ASSERT_MSG(scheduler_.is_running(), "scheduler not running");

			//
			//	thread safe access to task list
			//
			dispatcher_.dispatch([this, name, f]() {
				f(std::count_if(tasks_.begin(), tasks_.end(), [name](task_map::value_type const& v) {
					return boost::algorithm::equals(name, v.second->get_class_name());
				}));
			});
			return true;
		}

		bool mux::is_task(std::size_t id, std::function<void(bool)> f) const
		{
			if (shutdown_)	return false;
			BOOST_ASSERT_MSG(scheduler_.is_running(), "scheduler not running");
			
			//
			//	thread safe access to task list
			//
			dispatcher_.dispatch([this, id, f](){
				f(tasks_.find(id) != tasks_.end());
			});
			return true;
		}
		
		std::size_t mux::next_id()
		{
			return ++next_tag_;
		}
		
		void mux::shutdown()
		{
			scheduler_.stop(); 
		}
		
		bool mux::clear()
		{
			//
			//	We can shutdown the multiplexer only once.
			//	Posting messages or inserting new tasks no longer possible.
			//		
			if (!shutdown_.exchange(true))
			{
				//
				//	small optimization
				//
				if (tasks_.empty())
				{
					return true;
				}

				//
				//	Stop all tasks.
				//
				//	Send stop message to all remaining tasks in a separate thread
				//
				dispatcher_.dispatch([this]() {

					//
					//	reverse order - latest entries first
					//
					for (auto pos = tasks_.rbegin(); pos != tasks_.rend(); ++pos) {

						//
						//	call stop()
						//	This call is sync.
						//
						(*pos).second->stop(true);

					}

					//
					//	empty task table to signal that shutdown is complete
					//
					tasks_.clear();
				});

				return true;
			}

			return false;
		}

		void mux::stop(std::function<void(bool, std::size_t)> cb)
		{
			//
			//	We can shutdown the multiplexer only once.
			//	Posting messages or inserting new tasks no longer possible.
			//		
			if (!shutdown_.exchange(true))
			{
				//
				//	Stop all tasks.
				//
				//	Send stop message to all remaining tasks in a separate thread
				//
				dispatcher_.dispatch([this, cb]() {

					//
					//	reverse order - latest entries first
					//
					for (auto pos = tasks_.rbegin(); pos != tasks_.rend(); ++pos) {

						//
						//	call stop()
						//	This call is sync.
						//
						(*pos).second->stop(true);

					}

					//
					//	callback
					//
					cb(false, tasks_.size());

					//
					//	empty task table to signal that shutdown is complete
					//
					tasks_.clear();
				});

			}
			else {

				//
				//	failed
				//
				cb(false, tasks_.size());
			}
		}

		bool mux::stop(std::size_t id)
		{
			if (shutdown_)	return false;
			BOOST_ASSERT_MSG(scheduler_.is_running(), "scheduler not running");
			
			dispatcher_.post([this, id](){
				auto pos = tasks_.find(id);
				if (pos != tasks_.end())
				{
					(*pos).second->stop(false);
					tasks_.erase(pos);
				}
			});
			return true;
		}
		
		bool mux::stop(std::size_t id, std::function<void(bool, std::size_t)> f)
		{
			if (shutdown_)	return false;
			BOOST_ASSERT_MSG(scheduler_.is_running(), "scheduler not running");

			dispatcher_.post([this, id, f]() {
				auto pos = tasks_.find(id);
				if (pos != tasks_.end())	{
					f(true, (*pos).second->stop(false));
					tasks_.erase(pos);
				}
				else {
					f(false, 0);
				}
			});
			return true;
		}

		bool mux::stop(std::string const& name)
		{
			if (shutdown_)	return false;
			BOOST_ASSERT_MSG(scheduler_.is_running(), "scheduler not running");

			dispatcher_.dispatch([this, name]() {
				for (auto pos = tasks_.begin(); pos != tasks_.end(); /* empty */)
				{
					if (boost::algorithm::equals(name, (*pos).second->get_class_name()))	{
						(*pos).second->stop(false);
						pos = tasks_.erase(pos);
					}
					else {
						++pos;
					}
				}
			});

			return true;
		}

		bool mux::stop(std::string const& name, std::function<void(std::size_t)> f)
		{
			if (shutdown_)	return false;
			BOOST_ASSERT_MSG(scheduler_.is_running(), "scheduler not running");

			dispatcher_.dispatch([this, name, f]() {
				std::size_t counter{ 0 };
				for (auto pos = tasks_.begin(); pos != tasks_.end(); /* empty */)
				{
					if (boost::algorithm::equals(name, (*pos).second->get_class_name()))
					{
						(*pos).second->stop(false);
						pos = tasks_.erase(pos);
						++counter;
					}
					else {
						++pos;
					}
				}

				//
				//	callback
				//
				f(counter);
			});

			return true;
		}

		bool mux::insert(shared_task stp, sync)
		{
			//
			//	thread safe access to task list
			//
			if (mux::insert(stp, none()))
			{
				//
				//	same thread
				//
				stp->run();
				return true;
			}
			return false;
		}
		
		bool mux::insert(shared_task stp, detach)
		{
			if (shutdown_)	return false;
			BOOST_ASSERT_MSG(scheduler_.is_running(), "scheduler not running");
			
			//
			//	thread safe access to task list
			//
			dispatcher_.dispatch([this, stp](){
				if (tasks_.emplace_hint(tasks_.end(), stp->get_id(), stp) != tasks_.end())
				{
					//
					//	other thread
					//
					stp->run();
				}
			});
			return true;
		}
		
		bool mux::insert(shared_task stp, none)
		{
			if (shutdown_)	return false;
			BOOST_ASSERT_MSG(scheduler_.is_running(), "scheduler not running");
			
			//
			//	thread safe access to task list
			//
			dispatcher_.dispatch([this, stp]()->void {
				auto const r = tasks_.emplace_hint(tasks_.end(), stp->get_id(), stp) != tasks_.end();
			});
			return true;
		}
		

		void mux::post(std::size_t id, std::size_t slot, tuple_t&& msg) const
		{
			if (shutdown_)	return;
			BOOST_ASSERT_MSG(scheduler_.is_running(), "scheduler not running");

			//
			//	move the content of the message 
			//
			parameter param(std::move(msg));

			dispatcher_.dispatch([this, id, slot, param]() {
				auto pos = tasks_.find(id);
				if (pos != tasks_.end())
				{
					//
					//	async dispatching
					//
					(*pos).second->dispatch(slot, param.msg_);
				}
			});
		}
		
		void mux::post(std::string name, std::size_t slot, tuple_t&& msg) const
		{
			if (shutdown_)	return;
			BOOST_ASSERT_MSG(scheduler_.is_running(), "scheduler not running");

			//
			//	move the content of the message 
			//
			parameter param(std::move(msg));

			dispatcher_.dispatch([this, name, slot, param]() {
				for (auto pos = tasks_.begin(); pos != tasks_.end(); ++pos)
				{
					if (boost::algorithm::equals(name, (*pos).second->get_class_name()))
					{
						(*pos).second->dispatch(slot, param.msg_);
					}
				}
			});
		}

		void mux::post(std::size_t id, std::string slot, tuple_t&& msg) const
		{
			BOOST_ASSERT_MSG(scheduler_.is_running(), "scheduler not running");

			//
			//	move the content of the message 
			//
			parameter param(std::move(msg));

			dispatcher_.dispatch([this, id, slot, param]() {
				auto pos = tasks_.find(id);
				if (pos != tasks_.end())
				{
					//
					//	async dispatching
					//
					(*pos).second->dispatch(slot, param.msg_);
				}
			});
		}

		void mux::post(std::string slot, tuple_t&& msg) const
		{
			BOOST_ASSERT_MSG(scheduler_.is_running(), "scheduler not running");

			//
			//	move the content of the message 
			//
			parameter param(std::move(msg));

			dispatcher_.dispatch([this, slot, param]() {
				for (auto tsk : tasks_) {
					//
					//	async dispatching
					//
					tsk.second->dispatch(slot, param.msg_);
				}
			});
		}

		void mux::remove(std::size_t id)
		{
			if (!shutdown_)
			{
				BOOST_ASSERT_MSG(scheduler_.is_running(), "scheduler not running");
				dispatcher_.dispatch([this, id]() {
					tasks_.erase(id);
				});
			}
		}

		mux::parameter::parameter(tuple_t&& msg)
			: msg_(std::move(msg))
		{}

		mux::parameter::parameter(parameter const& other)
			: msg_(std::move(other.msg_))
		{}

		mux::parameter::parameter(parameter&& other)
			: msg_(std::move(other.msg_))
		{}

	}
}





