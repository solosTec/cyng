/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/async/mux.h>
#include <cyng/async/task/base_task.h>
#include <boost/algorithm/string/predicate.hpp>
#include <future>
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
		
		mux::mux(unsigned int count)
		: scheduler_(count)
		, dispatcher_(scheduler_.get_io_service())
		, next_tag_(NO_TASK)
		, tasks_()
		, shutdown_(false)
		{
			BOOST_ASSERT_MSG(scheduler_.is_running(), "scheduler not running");			
		}

		mux::~mux()
		{}
		
		std::size_t mux::size() const
		{
			if (shutdown_)	return 0u;
			BOOST_ASSERT_MSG(scheduler_.is_running(), "scheduler not running");
			
			//
			//	thread safe access to task list
			//
			std::promise<std::size_t> result;
			auto f = result.get_future();
			dispatcher_.dispatch([this, &result](){
				result.set_value(tasks_.size());
			});
			f.wait();
			return f.get();
		}
		
		std::size_t mux::size(std::string const& name) const
		{
			if (shutdown_)	return 0u;
			BOOST_ASSERT_MSG(scheduler_.is_running(), "scheduler not running");

			//
			//	thread safe access to task list
			//
			std::promise<std::size_t> result;
			auto f = result.get_future();
			dispatcher_.dispatch([this, name, &result]() {
				std::count_if(tasks_.begin(), tasks_.end(), [name](task_map::value_type const& v) {
					return boost::algorithm::equals(name, v.second->get_class_name());
				});
			});
			f.wait();
			return f.get();
		}

		bool mux::is_task(std::size_t id) const
		{
			if (shutdown_)	return false;
			BOOST_ASSERT_MSG(scheduler_.is_running(), "scheduler not running");
			
			//
			//	thread safe access to task list
			//
			std::promise<bool> result;
			auto f = result.get_future();
			dispatcher_.dispatch([this, id, &result](){
				result.set_value(tasks_.find(id) != tasks_.end());
			});
			f.wait();
			return f.get();
		}
		
		std::size_t mux::next_id()
		{
			return ++next_tag_;
		}
		
		void mux::shutdown()
		{
			scheduler_.stop(); 
		}
		
		bool mux::stop()
		{
			//
			//	We can shutdown the multiplexer only once
			//
			
			if (!shutdown_.exchange(true))
			{
				//std::cout << "mux::stop(#" << tasks_.size() << ")" << std::endl;	

				//
				//	stop all tasks
				//
				std::promise<bool> result;
				auto f = result.get_future();
				dispatcher_.dispatch([this, &result](){
					
					task_lst zombi_tasks;
					
					//
					//	reverse order - latest entries first
					//
					for (auto pos = tasks_.rbegin(); pos != tasks_.rend(); /* empty */) 
					{
						//
						//	call stop()
						//
						(*pos).second->stop();
						
						//
						//	move to zombi task
						//
						zombi_tasks.push_back((*pos).second);
						
						//
						//	remove from active task list
						//
						pos = decltype(pos){ tasks_.erase(std::next(pos).base()) };
						
						//
						//	enable thread switch
						//
						std::this_thread::yield();
					}
					
					//
					//	wait for pending references
					//
					while (true) 
					{
						bool b = std::any_of(zombi_tasks.begin(), zombi_tasks.end(), [](weak_task wtp){
							return !wtp.expired();
						});					
						
						if (b)
						{
							std::cerr << "MUST WAIT FOR SHUTDOWN " 
							<< std::count_if(zombi_tasks.begin(), zombi_tasks.end(), [this](weak_task wtp){
								auto tp = wtp.lock();
								if (tp)
								{
									std::cerr << "task $" << tp->get_id() << " is still alive" << std::endl;
// 									tp->stop();
									scheduler_.get_io_service().poll();
								}
								return !wtp.expired();				
							})
							<< std::endl;	
							std::this_thread::sleep_for(std::chrono::seconds(1));
						}
						else 
						{
							break;
						}
					}
					
					result.set_value(true);
				});

				f.wait();
				return f.get();
			}
			return false;
		}
		
		bool mux::stop(std::size_t id)
		{
			if (shutdown_)	return false;
			BOOST_ASSERT_MSG(scheduler_.is_running(), "scheduler not running");
			
			std::promise<bool> result;
			auto f = result.get_future();
			dispatcher_.dispatch([this, id, &result](){
				auto pos = tasks_.find(id);
				if (pos != tasks_.end())
				{
					(*pos).second->stop();
					tasks_.erase(pos);
					result.set_value(true);
				}
				else 
				{
					result.set_value(false);
				}
			});
			
			f.wait();
			return f.get();
		}
		
		std::size_t mux::stop(std::string const& name)
		{
			if (shutdown_)	return false;
			BOOST_ASSERT_MSG(scheduler_.is_running(), "scheduler not running");

			std::promise<std::size_t> result;
			auto f = result.get_future();
			dispatcher_.dispatch([this, name, &result]() {
				std::size_t counter{ 0 };
				for (auto pos = tasks_.begin(); pos != tasks_.end(); /* empty */)
				{
					if (boost::algorithm::equals(name, (*pos).second->get_class_name()))
					{
						(*pos).second->stop();
						pos = tasks_.erase(pos);
						++counter;
					}
					else
					{
						++pos;
					}
				}

				//
				//	set future value
				//
				result.set_value(counter);
			});

			f.wait();
			return f.get();
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
		
		void mux::insert(shared_task stp, detach)
		{
			if (shutdown_)	return;
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
		}
		
		bool mux::insert(shared_task stp, none)
		{
			if (shutdown_)	return false;
			BOOST_ASSERT_MSG(scheduler_.is_running(), "scheduler not running");
			
			//
			//	thread safe access to task list
			//
			std::promise<bool> result;
			auto f = result.get_future();
			dispatcher_.dispatch([this, stp, &result](){
// 				std::cout << "insert-none: " << stp->get_id() << std::endl;	
				result.set_value(tasks_.emplace_hint(tasks_.end(), stp->get_id(), stp) != tasks_.end());
			});
			f.wait();
			return f.get();
		}
		
		bool mux::send(std::size_t id, std::size_t slot, tuple_t&& msg) const
		{
			if (shutdown_)	return false;
			BOOST_ASSERT_MSG(scheduler_.is_running(), "scheduler not running");
			
			std::promise<bool> result;
			auto f = result.get_future();
			dispatcher_.dispatch([this, id, slot, &msg, &result](){
				auto pos = tasks_.find(id);
				if (pos != tasks_.end())
				{
					(*pos).second->dispatch(slot, msg);
					result.set_value(true);
				}
				else 
				{
					result.set_value(false);
				}
			});
			
			f.wait();
			return f.get();
		}
		
		void mux::remove(std::size_t id)
		{
			dispatcher_.dispatch([this, id]() {
				tasks_.erase(id);
			});
		}
	}
}





