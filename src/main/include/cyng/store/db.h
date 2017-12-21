/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_STORE_DB_H
#define CYNG_STORE_DB_H


#include <cyng/store/table.h>
#include <cyng/store/transform.hpp>

#ifdef _DEBUG
#include <boost/core/demangle.hpp>
#endif

namespace cyng 
{
	namespace store 
	{
		/**
		 * A store consist of a list of named tables. Each table has a consistent
		 * structure, described in a special structure named meta.
		 * A tables containts records consisting of keys and values.
		 * 
		 * Internally a store is a table of tables. This offers a natural way
		 * to use all table (lock) mechanisms for the database too.
		 */
		class db
		{
			
		public:
			db();
			
			/**
			 * Destructor - remove all data
			 */
			virtual ~db();
			
			/**
			 * create and insert a new table
			 * @return true if table was actually inserted.
			 */
			bool create_table(meta_table_ptr);
			
			/**
			 * @return number of tables
			 */
			std::size_t size() const;
			
			/**
			 * generic access on tables
			 */
#if defined(CYNG_LEGACY_MODE_ON)
			template <class F, typename ...Tbls>
// 			constexpr auto 
			void
// 			access(F&& f, std::tuple<Tbls...>& tbls) -> typename  boost::fusion::result_of::invoke_function_object<F,std::tuple<Tbls...> >::type
			access(F f, Tbls&& ... tbls)
#else
			template <class F, typename ...Tbls>
			constexpr decltype(auto)  
			access(F&& f, Tbls&& ... tbls)
#endif
			{
				//
				//	read lock on db
				//
				cyng::async::shared_lock<cyng::async::shared_mutex> sl(this->m_);

				//
				//	get the tuple with table pointers
				//
				auto tbl_list = get_table_ptrs(std::forward<Tbls>(tbls)...);
				
// 				std::tuple<cyng::store::table const*, cyng::store::table*>
// 				std::cout
// 				<< boost::core::demangle(typeid(decltype(tbl_list)).name())
// 				<< std::endl;
				
				//
				//	Generate a tuple with the appropriate lock types
				//	read/write lock tables
                //  lock_list is a tuple of boost::shared_lock<boost::shared_mutex> and
                //  boost::unique_lock<boost::shared_mutex>.
				//
				auto lock_list = get_locks(std::forward<Tbls>(tbls)...);
				
				//
				//	Now we have a tuple with defered locks and have to do 
				//	the final lock for all.
				//
				cyng::meta::apply(sync_functor(), lock_list);
				
				//
				//	call function F with unpacked table pointers
				//
#if defined(CYNG_LEGACY_MODE_ON)
				cyng::meta::apply(f, tbl_list);
#else
				return cyng::meta::apply(std::forward<F>(f), tbl_list);
#endif
				
				//
				//	implicit unlock of all affected tables
				//
			}
			
		private:
			/**
			 * Create a tuple of matching table pointer types 
			 * and fill this tuple with the existing tables.
			 */
			template <typename ...Tbls>
			auto get_table_ptrs(Tbls&&... tbls) -> typename calculate_ptr_type<std::tuple, Tbls...>::type
			{
				//
				//	compile time calculation: tuple of const and non-const pointers
				//	of type table.
				//
				using tbl_list_t = typename calculate_ptr_type<std::tuple, Tbls...>::type;
				tbl_list_t tbl_list;

				//
				//	runtime calculation: get all table pointers of tables
				//	with the specified names.
				//
				table_functor<tbl_list_t> f(tbl_list, this->tables_);
				tbl_loop<Tbls...>::invoke(f, std::forward<Tbls>(tbls)...);
				
				return tbl_list;
			}
			
			/**
			 * Create a tuple of matching lock types and set a lock for the specified tables.
			 */
			template <typename ...Tbls>
			auto get_locks(Tbls&&... tbls) -> typename calculate_lock_type<std::tuple, Tbls...>::type
			{
				//
				//	compile time calculation
				//
				using lock_list_t = typename calculate_lock_type<std::tuple, Tbls...>::type;
				lock_list_t lock_list;

				//	debug output
// 				get_locks: std::tuple<std::shared_lock<std::shared_mutex>, std::unique_lock<std::shared_mutex> >
// 				std::cout 
// 				<< "get_locks: " 
// 				<< boost::core::demangle(typeid(lock_list_t).name())
// 				<< std::endl;
				
				//
				//	runtime calculation
				//	Loop over lock list and and set a defered lock
				//
 				lock_functor<lock_list_t> f(lock_list, this->tables_);
				tbl_loop<Tbls...>::invoke(f, std::forward<Tbls>(tbls)...);
				
				return lock_list;
			}
			
			
		/**
		 * forward declaration.
		 * global function to get a const pointer to a table from
		 * this database layout
		 * 
		 * @return a pair containing a const pointer to the requested table
		 * and there mutex. If no table was found both pointers are null.
		 * pointer with the specified name or a nullptr.
		 * 
		 * To detect such empty pairs use the helper function is_empty().
		 * 
		 * @see is_empty()
		 */
// 		std::pair<table const*, mutex_t*> 
// 		find_table(const table&, std::string const& name);
			
		private:
			/**
			 * contains all tables of the database 
			 */
			table tables_;
			
			/**
			 * A sync object for this database
			 */
			mutable cyng::async::shared_mutex m_;
			
		};
		
	}	//	store	
	
	namespace traits
	{
		template <>
		struct type_tag<cyng::store::db>
		{
			using type = cyng::store::db;
			using tag =  std::integral_constant<std::size_t, PREDEF_DB>;
#if defined(CYNG_LEGACY_MODE_ON)
			const static char name[];
#else
			constexpr static char name[] = "db";
#endif
		};
		
		template <>
		struct reverse_type < PREDEF_DB >
		{
			using type = cyng::store::db;
		};
		
	}
	
}

#endif	//	CYNG_STORE_DB_H






