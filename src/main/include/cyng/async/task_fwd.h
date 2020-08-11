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
#include <vector>
#include <memory>
#include <limits>

namespace cyng 
{
	namespace async 
	{
		/**
		 * 0 is an invalid task id
		 */
		constexpr std::size_t NO_TASK = 0;
		constexpr std::size_t NO_SLOT = std::numeric_limits<std::size_t>::max();

		//	some forward definitions
		class mux;
 		class base_task;
		
		using shared_task = std::shared_ptr< base_task >;
		using weak_task = std::weak_ptr< base_task >;
		using task_map_t = std::map< std::size_t, shared_task >;
		using task_list_t = std::vector< std::size_t >;

	}	// async
}

#endif	//	CYNG_ASYNC_TASK_FWD_H



