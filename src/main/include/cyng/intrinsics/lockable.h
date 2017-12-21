/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_INTRINSICS_LOCKABLE_H
#define CYNG_INTRINSICS_LOCKABLE_H


#include <cyng/compatibility/async.h>
#include <cyng/object.h>
#include <utility>

namespace cyng 
{
	
	/**
	 * Since a (shared) mutex is neither copyable nor moveable
	 * a special class/struct that don't try to copy or move 
	 * the mutex during assignment or copy construction.
	 */
	struct lockable
	{
		object obj_;
		mutable async::shared_mutex m_;
		
		/**
		 * Each lock-able object has an generation counter 
		 */
		std::uint64_t generation_;

		lockable();
		lockable(object&&, std::uint64_t);
		lockable(lockable&&);
		
		/**
		 * thread safe assignment
		 */
		lockable& operator=(lockable const& other);
		lockable& operator=(lockable&& p);
				
	};
}

#endif 	// CYNG_INTRINSICS_LOCKABLE_H

