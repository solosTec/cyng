/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_ASYNC_TASK_FWD_H
#define CYNG_ASYNC_TASK_FWD_H

#include <cstddef>
#include <cyng/compatibility/legacy_mode.hpp>
#include <map>
#include <list>
#include <memory>

namespace cyng 
{
	namespace async 
	{
		/**
		 * 0 is an invalid task id
		 */
		constexpr std::size_t NO_TASK = 0;
		
		//	some forward definitions
		class mux;
 		class base_task;
		
		using shared_task = std::shared_ptr< base_task >;
		using weak_task = std::weak_ptr< base_task >;
		using task_map = std::map< std::size_t, shared_task >;
		using task_lst = std::list< weak_task >;
		

	}	// async
}

#endif	//	CYNG_ASYNC_TASK_FWD_H



