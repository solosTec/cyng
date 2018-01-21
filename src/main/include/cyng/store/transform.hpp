/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_STORE_TRANSFORM_HPP
#define CYNG_STORE_TRANSFORM_HPP

#include <cyng/store/table.h>
#include <cyng/meta/meta.hpp>
#include <cyng/compatibility/async.h>
#include <cyng/object_cast.hpp>

#ifdef _DEBUG
#include <boost/core/demangle.hpp>
#endif

namespace cyng 
{
	namespace store 
	{
		
		
		/**
		 * Encodes the request for a read lock
		 */
		struct read_access
		{
			read_access(std::string const& name)
			: table_name_(name)
			{}
			const std::string table_name_;
		};
		
		/**
		 * Encodes the request for a write lock
		 */
		struct write_access
		{
			write_access(std::string const& name)
			: table_name_(name)
			{}
			const std::string table_name_;
		};
		
		namespace //	anonymous
		{
			/**
			 * get the appropriate lock type 
			 */
			template < typename T >
			struct get_lock_type_impl
			{};

			template <>
			struct get_lock_type_impl< read_access >
			{
				using type = shared_lock_t;
			};

			template <>
			struct get_lock_type_impl< write_access >
			{
				using type = unique_lock_t;
			};

			/**
			 * get the appropriate parameter type: 
			 * 
			 * @code
			 * P* ...
			 * P const* ...
			 * @endcode
			 */
			template < typename T, typename P >
			struct get_param_type_impl
			{};

			template <typename P>
			struct get_param_type_impl< read_access, P >
			{
				using type = typename std::decay<P>::type;
				using ptr_type = typename std::add_pointer< typename std::add_const< type >::type >::type;
			};

			template <typename P>
			struct get_param_type_impl< write_access, P >
			{
				using type = typename std::decay<P>::type;
				using ptr_type = typename std::add_pointer< type >::type;
			};
		}
		
		/**
		 * alias for table pointer types
		 */
		template< typename T >
		using get_table_ptr_type = typename get_param_type_impl< T, table >::ptr_type;
		

		/**
		 * calculate the table pointer types according to the specified
		 * list of read_access and write_access classes.
		 */
		template <template<class...> class R, typename ...Tbls>
		struct calculate_ptr_type
		{
			using type = cyng::meta::transform<get_table_ptr_type, R<Tbls...>>;
		};
		
		/**
		 * alias for table lock types
		 */
		template< typename T >
		using get_lock_type = typename get_lock_type_impl< T >::type;
		
		/**
		 * calculate the lock types according to the specified
		 * list of table_read and table_write classes.
		 */
		template <template<class...> class R, typename ...Tbls>
		struct calculate_lock_type
		{
			using type = cyng::meta::transform<get_lock_type, R<Tbls...>>;
		};
		
		/**
		 * The table functor has to provide a method named call, that is
		 * templatized with a numerical type N and an type U, that is the 
		 * table implementation.
		 */
// 		template < template<class...> class R, typename ...Tbls>
		template < typename T >
		struct table_functor
		{
			T& result_;
// 			R<Tbls...> result_;
// 			std::tuple<Tbls...>	result_;
			const table& db_;
			
// 			table_functor(std::tuple<Tbls...>& r, const table& db)
			table_functor(T& r, const table& db)
			: result_(r)
			, db_(db)
			{}

			template<std::size_t N, typename U>
			void call (U&& tbl)
			{
// 				table_functor(0, cyng::store::read_access, cyng::store::table const*, table-1)

// 				std::cout 
// 				<< "table_functor("
// 				<< N
// 				<< ", "
// 				<< boost::core::demangle(typeid(U).name()) 
// 				<< ", "
// 				<< boost::core::demangle(typeid(decltype(std::get<N>(result_))).name()) 
// 				<< ", "
// 				<< tbl.table_name_
// 				<< ")"
// 				<< std::endl;

// 				std::pair<table_type::const_iterator,bool> find(key_type const& key) const;
				std::pair<table::table_type::const_iterator, bool> r = db_.find(cyng::table::key_generator(tbl.table_name_));
				if (r.second)
				{
// 					std::pair<std::vector<cyng::object, std::allocator<cyng::object> > const, cyng::lockable>
// 					std::cout 
// 					<< boost::core::demangle(typeid(decltype(*r.first)).name())
// 					<< std::endl;
					const cyng::table::data_type* dp = object_cast<cyng::table::data_type>((*r.first).second.obj_);
					
// 					std::vector<cyng::object, std::allocator<cyng::object> >
// 					std::cout 
// 					<< boost::core::demangle(typeid(decltype(*dp)).name())
// 					<< std::endl;
					
					std::get<N>(result_) = const_cast<table*>(object_cast<table>((*dp)[0]));

					//std::cout 
					//	<< "functor< " 
					//	<< N 
					//	<< " > :: " 
					//	<< (std::get<N>(result_) ? std::get<N>(result_)->meta().get_name() : "NULL")
					//	<< std::endl;
					
				}
				else
				{
					std::get<N>(result_) = nullptr;
					std::cerr 
					<< "table "
					<< tbl.table_name_
					<< " not found"
					<< std::endl;
				}
			}			
		};
		
		/**
		 * The lock functor has to provide a method named call, that is
		 * templatized with a numerical type N and an type U, that is the 
		 * table implementation.
		 */
		template < typename T >
		struct lock_functor
		{
			T& result_;
			const table& db_;
			
			lock_functor(T& r, const table& db)
			: result_(r)
			, db_(db)
			{}
			
			template<std::size_t N, typename U>
			void call (U&& tbl)
			{
				using lock_type = typename std::decay<decltype(std::get<N>(result_))>::type;

// 				lock_functor(0, cyng::store::read_access, std::shared_lock<std::shared_mutex>, table-1)
// 				lock_functor(1, cyng::store::write_access, std::unique_lock<std::shared_mutex>, table-2)
// 				std::cout 
// 				<< "lock_functor("
// 				<< N
// 				<< ", "
// 				<< boost::core::demangle(typeid(U).name()) 
// 				<< ", "
// 				<< boost::core::demangle(typeid(decltype(std::get<N>(result_))).name()) 
// 				<< ", "
// 				<< tbl.table_name_
// 				<< ")"
// 				<< std::endl;
				
				std::pair<table::table_type::const_iterator, bool> r = db_.find(cyng::table::key_generator(tbl.table_name_));
				if (r.second)
				{
// 					std::pair<std::vector<cyng::object, std::allocator<cyng::object> > const, cyng::lockable>
// 					std::cout 
// 					<< boost::core::demangle(typeid(decltype(*r.first)).name())
// 					<< std::endl;
					
// 					const data_type* dp = object_cast<data_type>((*r.first).second.obj_);
// 					&(*r.first).second.mutex_
// 					(*r.first).second.m_; - works
					std::get<N>(result_) = lock_type((*r.first).second.m_, cyng::async::defer_lock);
					
				}
				else
				{
					std::cerr 
					<< "table "
					<< tbl.table_name_
					<< " not found"
					<< std::endl;
				}
			}			
		};
		
		template<typename F, std::size_t N, typename ...Args> 
		struct tbl_loop_impl
		{
			typedef typename std::tuple_element<N, std::tuple<Args...>>::type U;
			
			static void invoke(F f, Args&&... args)
			{
				//	recursive call
				tbl_loop_impl<F, N - 1, Args...>::invoke(f, std::forward<Args>(args)...);
				
				//
				//	std::get< N >() return a value of type U
				//
				//	N is provides as template argument to the called function.
				//	This offers a path to use the index of the tuple element
				//	at compile time.
				//
				//const U value = std::get< N >(std::forward_as_tuple(std::forward<Args>(args)...));
				f.template call<N>(std::get< N >(std::forward_as_tuple(std::forward<Args>(args)...)));
			}
			
		};
		
		template<typename F, typename ...Args> 
		struct tbl_loop_impl< F, 0, Args... >
		{
			typedef typename std::tuple_element<0, std::tuple<Args...>>::type U;
			
			static void invoke(F f, Args&&... args)
			{
				f.template call<0>(std::get< 0 >(std::forward_as_tuple(std::forward<Args>(args)...)));
			}
		};
		
		/**
		 * Iterate over a variadic template parameter pack and
		 * call a functor/function for each element.
		 * 
		 * Example:
		 * @code
			struct my_functor
			{
				template<typename T>
				void operator () (T&& t)
				{
					std::cout 
					<< "my_functor("
					<< demangle(typeid(T).name()) 
					<< ")"
					<< std::endl;
				}
			};
	
			template <typename ...Args>
			void calculate(Args&&... args)
			{
				tbl_loop<Args...>::invoke(my_functor(), std::forward<Args>(args)...);
			}
		 * @endcode
		 */
		template<class ...Args> 
		struct tbl_loop
		{
			using size = std::integral_constant<std::size_t, sizeof...(Args)>;
			
			template < typename F >
			static void invoke(F f, Args&&... args)
			{
				typedef tbl_loop_impl<F, size::value-1, Args...> impl;
				impl::invoke(f, std::forward<Args>(args)...);
			}
		};
	
		template<template<class...> class T, class... Args> 
		struct tbl_loop<T<Args...>>
		{
			//
			//	Extract the variadic template parameter pack
			//	and invoke the recursive implementation.
			//
			template < typename F >
			static void invoke(F f, Args&&... args)
			{
				tbl_loop<Args...>::invoke(f, std::forward<Args>(args)...);
			}
		};
		
		
		struct sync_functor
		{
           
            //
            //  rvalues of boost::shared_lock<boost::shared_mutex> and
            //  boost::unique_lock<boost::shared_mutex>
            //
			template<typename ...Args>
			void operator() (Args&&... args)
			{
//  				std::cout 
//  				<< "sync_functor("
//  				<< sizeof...(Args)
//  				<< ")"
//  				<< std::endl;
				
// 				std::lock(std::forward<Args>(args)...);
//  				cyng::async::lock<Args...>(std::forward<Args>(args)...);
  				cyng::async::lock(std::forward<Args>(args)...);
			}

			/*
			 * single element
			 */
			template<typename T>
			void operator() (T& mtx)
			{
				mtx.lock();
			}

		};
	}
}

#endif //	CYNG_STORE_TRANSFORM_HPP
