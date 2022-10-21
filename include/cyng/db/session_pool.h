/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#ifndef CYNG_DB_SESSION_POOL_H
#define CYNG_DB_SESSION_POOL_H


#include <cyng/db/session.h>
//#include <cyng/compatibility/async.h>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>

#include <chrono>
#include <list>
#include <shared_mutex>
#include <mutex>

namespace cyng
{
	namespace db 
	{

		/**	
		 * @brief Manage a session pool
		 *
		 */
		class session_pool
		{
			//typedef cyng::async::shared_mutex mutex_t;

		public:
			session_pool(boost::asio::io_service&, connection_type);
			session_pool(session_pool const&) = delete;

			virtual ~session_pool();
			
			/**
			 * Takes a session from the idle pool and moves it into the 
			 * pool of running sessions.
			 * It is the responsibility of the user to close this session.
			 */
			session get_session();
			
			/**
			 * Build up session pool.
			 * 
			 * Pool parameters are:
			 * <ul>
			 * <li>"pool-size" requested pool size</li>
			 * <li>"life-time" life time for idle connections in seconds</li>
			 * </ul>
			 *
			 * @return true if pool is complete
			 */
			bool start(param_map_t const& config);
			bool start(tuple_t const& config);

			/**
			 * @brief Build up session pool 
			 * 
			 * @param size pool size (2 should be minimum)
			 * @param lifetime lifetime of a connection in seconds if idle
			 * @return true if pool is complete
			 */
			bool start(std::size_t size
			, std::chrono::seconds lifetime
			, param_map_t const& config);

			/**
			 * Stop timer and closes all idle connections
			 */
			void stop();

			/**
			 * @return the real pool size
			 */
			std::size_t get_pool_size() const;

			/*
			 * @see pool_size_
			 * @return true if pool size is one.
			 */
			bool is_single_mode() const;

			/**
			 * @return the life time of idle sessions
			 */
			std::chrono::seconds get_life_time() const;


		private:
			std::pair<std::string, bool> add_session();
			void start_timer();
			void on_timer(boost::system::error_code const&);
				
		private:
			boost::asio::steady_timer timer_;
			connection_type ctype_;
			std::list<session>	pool_;
			mutable std::shared_mutex mutex_;

			/*
			 * File-based databases like SQLite don't support multiple connections at the same time 
			 * very well. So the pool contains only one connection.
			 */
			std::size_t pool_size_;
			std::chrono::seconds life_time_;
			param_map_t config_;

		};

	}	//	db
}	//	cyng

#endif	//	CYNG_DB_SESSION_POOL_H
