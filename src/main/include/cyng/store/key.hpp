/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_STORE_KEY_HPP
#define CYNG_STORE_KEY_HPP

#include <cyng/intrinsics/sets.h>
#include <cyng/factory.h>

namespace cyng 
{
	namespace store 
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
		 const cyng::store::key_type key = cyng::store::key_generator( "1", 2, std::chrono::seconds(3));
		 *	@endcode
		 */
		template < typename ...Args >
		key_type key_generator(Args&&... args)
		{
			return key_maker<Args...>()(std::forward<Args>(args)...);
		}
		
		/**
		 * An explicit structure to define a table key with attributes (like generation counter)
		 */
// 		struct key
// 		{
// 			/**
// 			 * the primary key is a vector of sortable objects 
// 			 */
// 			key_type	pk_;
// 			
// 			/**
// 			 * Each key provide a generation counter as a replacement for 
// 			 * timestamps. Newer version of a record have a higher generation
// 			 * counter.
// 			 */
// 			std::uint64_t	generation_;
// 			
// 			key();
// 			
// 			template < typename ...Args >
// 			key(Args&&... args)
// 			: pk_(key_generator<Args...>(std::forward<Args>(args)...))
// 			, generation_(0)
// 			{}
// 
// 			template < typename ...Args >
// 			key(std::uint64_t gen, Args&&... args)
// 				: pk_(key_generator<Args...>(std::forward<Args>(args)...))
// 				, generation_(gen)
// 			{}
// 		};

		/*
		 * Key generator allows to specify the generation.
		 */
// 		template < typename ...Args >
// 		key make_key(std::uint64_t gen, Args&&... args)
// 		{
// 			return key(gen, std::forward<Args>(args)...);
// 		}
	}	//	store	

	namespace traits
	{
// 		template <>
// 		struct type_tag<cyng::store::key>
// 		{
// 			using type = cyng::store::key;
// 			using tag = std::integral_constant<std::size_t, PREDEF_KEY>;
// #if defined(CYNG_LEGACY_MODE_ON)
// 			const static char name[];
// #else
// 			constexpr static char name[] = "key";
// #endif
// 		};
// 
// 		template <>
// 		struct reverse_type < PREDEF_KEY >
// 		{
// 			using type = cyng::store::key;
// 		};
	}
}

#include <functional>

namespace std
{
// 	template<>
// 	struct hash<cyng::store::key>
// 	{
// 		size_t operator()(cyng::store::key) const noexcept;
// 	};
// 
// 	template<>
// 	struct equal_to<cyng::store::key>
// 	{
// 		using result_type = bool;
// 		using first_argument_type = cyng::store::key;
// 		using second_argument_type = cyng::store::key;
// 
// 		bool operator()(cyng::store::key const& c1, cyng::store::key const& c2) const noexcept;
// 	};

}

#endif	//	CYNG_STORE_KEY_HPP


