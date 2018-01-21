/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_ASYNC_H
#define CYNG_ASYNC_H

#include <CYNG_project_info.h>

//
//	define synchronisation objects
//
#if defined(CYNG_STD_SHARED_MUTEX_OFF)
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/lock_options.hpp>
#include <boost/thread/future.hpp>

namespace cyng 
{
	namespace async 
	{
		using mutex = boost::mutex;
		using shared_mutex = boost::shared_mutex;
		
		template < typename Mutex >
		using unique_lock = boost::unique_lock< Mutex >;

		template < typename Mutex >
		using shared_lock = boost::shared_lock< Mutex >;
		
		template < typename Mutex >
		using lock_guard = boost::lock_guard< Mutex >;
		
		template < typename L1, typename L2, typename ...Ln >
		inline void lock(L1& l1, L2& l2, Ln&... ln)
		{
			boost::lock<L1, L2, Ln...>(l1, l2, ln...);
		}
		
		BOOST_CONSTEXPR_OR_CONST boost::defer_lock_t defer_lock = {};
		BOOST_CONSTEXPR_OR_CONST boost::try_to_lock_t try_to_lock = {};
		BOOST_CONSTEXPR_OR_CONST boost::adopt_lock_t adopt_lock = {};
		
		template < typename Mutex >
		using promise = boost::promise< Mutex >;
	
		using condition_variable = boost::condition_variable;
		
	}
}

#else // defined(CYNG_STD_SHARED_MUTEX_OFF)


#include <shared_mutex>
#include <mutex>
#include <future>

namespace cyng 
{
	namespace async 
	{
		using mutex = std::mutex;
		using shared_mutex = std::shared_mutex;
		
		template < typename Mutex >
		using unique_lock = std::unique_lock< Mutex >;

		template < typename Mutex >
		using shared_lock = std::shared_lock< Mutex >;
		
		template < typename Mutex >
		using lock_guard = std::lock_guard< Mutex >;
		
		
		template < typename L1, typename L2, typename ...Ln >
		inline void lock(L1& l1, L2& l2, Ln&... ln)
		{
			std::lock(l1, l2, ln...);
		}

 		constexpr std::defer_lock_t defer_lock {};
 		constexpr std::try_to_lock_t try_to_lock {};
 		constexpr std::adopt_lock_t adopt_lock {};
		
		template < typename Mutex >
		using promise = std::promise< Mutex >;
		
		using condition_variable = std::condition_variable;
		
	}
}
#endif
#endif 	//	CYNG_ASYNC_H




