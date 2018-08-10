/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Sylko Olzscher
 *
 */


#include <cyng/db/session_pool.h>
#include <cyng/dom/algorithm.h>
#include <cyng/set_cast.h>
#include <iostream>
#include <boost/bind.hpp>

namespace cyng
{
	namespace db 
	{	
		session_pool::session_pool(boost::asio::io_service& service, connection_type ctype)
		: timer_(service)
		, ctype_(ctype)
		, pool_()
		, mutex_()
		, pool_size_((ctype == CONNECTION_SQLITE) ? 1 : 5)
		, life_time_(120)
		, config_()
		{}
		
		session_pool::~session_pool()
		{}
	
		session session_pool::get_session()
		{
			cyng::async::unique_lock<mutex_t> rls(mutex_);
			
			if (!pool_.empty())
			{
				if (pool_size_ == 1)
				{
					return pool_.front();
				}
				else
				{
					//	create a new one at the end 
					//	of the list
					add_session();

					//
					//	and remove first (oldest) session
					session s(std::move(pool_.front()));	//	move constructor
					pool_.pop_front();
					return s;
				}
			}
			return session(ctype_);
		}
	
		bool session_pool::start(param_map_t const& config)
		{
			return start(find_value(config, "pool-size", pool_size_) 
			, find_value(config, "life-time", life_time_)
			, config);
		}
		
		bool session_pool::start(tuple_t const& config)
		{
			const param_map_t pmap = to_param_map(config);

			return start(find_value(pmap, "pool-size", static_cast<std::int32_t>(pool_size_))
				, find_value(pmap, "life-time", life_time_)
				, pmap);
		}

		bool session_pool::start(std::size_t pool_size
			, std::chrono::seconds life_time
			, param_map_t const& config)
		{
			if (pool_size < 1)	return false;

#ifdef _DEBUG
			if (ctype_ == CONNECTION_SQLITE)
			{
				BOOST_ASSERT(pool_size == 1);
			}
#endif
			pool_size_ = pool_size;
			if (life_time_ < std::chrono::seconds(5))	return false;

			life_time_ = life_time;
			config_ = config;

			//	exclusive lock pool
			cyng::async::unique_lock<mutex_t> rls(mutex_);
			while (pool_size-- > 0)
			{
				add_session();
			}

			if (pool_size_ > 1)
			{
				start_timer();
			}
			return pool_.size() == pool_size_;
		}
		
		std::pair<std::string, bool> session_pool::add_session()
		{
			session s(ctype_);
			const std::pair<std::string, bool> r = s.connect(config_);
			if (r.second)
			{
				pool_.emplace_back(std::move(s));
			}
			else 
			{
				std::cerr 
					<< '\n'
					<< r.second 
					<< '\n'
					<< std::endl;
			}
			return r;
		}
		
		void session_pool::on_timer(boost::system::error_code const& ec)
		{
			if (ec != boost::asio::error::operation_aborted)
			{
				BOOST_ASSERT_MSG(pool_.size() <= pool_size_, "internal error");
				
				//	reshuffle pool
				cyng::async::unique_lock<mutex_t> rls(mutex_);
				if (pool_.size() == pool_size_)
				{
					//	shuffle one session
					//	add a session at the end
					add_session();

					//	and remove first (oldest) session
					session s(std::move(*pool_.begin()));	//	move constructor
					pool_.pop_front();
				}
				else 
				{
					//	refill pool
					std::size_t diff = pool_size_ - pool_.size();
					while (diff-- > 0)
					{
						add_session();
					}
				}
				
				//	restart timer
				start_timer();
			}
		}
	
		void session_pool::start_timer()
		{
			timer_.expires_from_now(life_time_);
			timer_.async_wait(boost::bind(&session_pool::on_timer, this, boost::asio::placeholders::error));
		}
	
		void session_pool::stop()
		{
			timer_.cancel();
			cyng::async::unique_lock<mutex_t> rls(mutex_);
			for (auto s : pool_)
			{
				s.close();
			}
			pool_.clear();
		}

		std::size_t session_pool::get_pool_size() const
		{
			cyng::async::unique_lock<mutex_t> rls(mutex_);
			return pool_.size();
		}

		bool session_pool::is_single_mode() const
		{
			return get_pool_size() == 1;
		}

		/**
		 * @return the life time of idle sessions
		 */
		std::chrono::seconds session_pool::get_life_time() const
		{
			return life_time_;
		}

	}
}

