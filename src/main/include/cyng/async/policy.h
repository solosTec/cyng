/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_ASYNC_POLICY_H
#define CYNG_ASYNC_POLICY_H

namespace cyng 
{
	namespace async 
	{
		/**
		 * distinguish between public no thread safe methods
		 * and private thread safe methods
		 */
		struct detach
		{};
		struct sync
		{};
		struct none
		{};
	}
}

#endif	//	CYNG_ASYNC_POLICY_H



