/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_STORE_BODY_HPP
#define CYNG_STORE_BODY_HPP

#include <cyng/store/key.hpp>
#include <cyng/intrinsics/lockable.h>

namespace cyng 
{
	namespace store 
	{

		/**
		 * We have to break a rule here. The vector may contain objects
		 * of different data types. But we need the advantage of vectors
		 * to get fast access using the subscription operator []. 
		 */
		using data_type = key_type;
		using body_type = lockable;

		/**
		 *	Table data generator accepts an arbitrary list of object types.
		 *
		 *	example:
		 *	@code
		 const cyng::store::data_type key = cyng::store::data_generator( "1", 2, std::chrono::seconds(3));
		 *	@endcode
		 */
		template < typename ...Args >
		data_type data_generator(Args&&... args)
		{
			return key_generator(std::forward<Args>(args)...);
		}
		
		template < typename ...Args >
		lockable body_generator(Args&&... args)
		{
			return lockable(make_object(data_generator(std::forward<Args>(args)...)), 0);
		}
		
		inline lockable body_generator(data_type&& data, std::uint64_t generation)
		{
			return lockable(make_object(std::move(data)), generation);
		}
		
		
		
	}	//	store	
}

#endif	//	CYNG_STORE_BODY_HPP


