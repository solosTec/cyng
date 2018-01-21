/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_TABLE_KEY_HPP
#define CYNG_TABLE_KEY_HPP

#include <cyng/intrinsics/sets.h>
#include <cyng/factory.h>

namespace cyng 
{
	namespace table 
	{

		/**
		 * We have to break a rule here. The vector may contain objects
		 * of different data types. But we need the advantage of vectors
		 * to get fast access using the subscription operator []. 
		 */
		using key_type = vector_t;
		using key_list_t = std::list<key_type>;

		namespace
		{
			template < typename ...Args >
			struct key_maker
			{
				key_type operator()(Args&&... args) const
				{
					//	create a tuple
					const tuple_t tpl = tuple_factory(std::forward<Args>(args)...);
					BOOST_ASSERT(tpl.size() == sizeof...(Args));

					//	convert to vector
					key_type key;
					key.reserve(tpl.size());
					key.insert(key.begin(), tpl.begin(), tpl.end());
					return key;
				}
			};

			template < typename ...Args >
			struct key_maker< std::tuple< Args... >>
			{
				typedef typename std::tuple< Args... >	tpl_type;

				key_type operator()(tpl_type&& arg) const
				{
					//	create a tuple
					const tuple_t tpl = tuple_factory(std::forward<tpl_type>(arg));
					BOOST_ASSERT(tpl.size() == sizeof...(Args));

					//	convert to vector
					key_type key;
					key.reserve(tpl.size());
					key.insert(key.begin(), tpl.begin(), tpl.end());
					return key;
				}
			};
		}

		/**
		 *	Table key generator accepts an arbitrary list of object types.
		 *
		 *	example:
		 *	@code
		 const cyng::table::key_type key = cyng::store::key_generator( "1", 2, std::chrono::seconds(3));
		 *	@endcode
		 */
		template < typename ...Args >
		key_type key_generator(Args&&... args)
		{
			return key_maker<Args...>()(std::forward<Args>(args)...);
		}
		
	}	//	table	
}


#endif	//	CYNG_TABLE_KEY_HPP


