/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_STORE_TABLE_H
#define CYNG_STORE_TABLE_H


#include <cyng/store/meta_interface.h>
#include <cyng/store/publisher.h>
#include <cyng/store/record.h>
#include <unordered_map>
#include <cyng/intrinsics/traits.hpp>
#include <functional>

namespace cyng 
{
	namespace store 
	{
		//
		//	alias for lock types
		//
		using mutex_t 		= cyng::async::shared_mutex;
		using shared_lock_t = cyng::async::shared_lock<mutex_t>;
		using unique_lock_t = cyng::async::unique_lock<mutex_t>;
		
		class table : public publisher
		{
// 			template < template<class...> class R, typename ...Tbls>
			template < typename T >
			friend struct table_functor;
			template < typename T >
			friend struct lock_functor;
			
		public:
			using table_type = std::unordered_map< key_type, body_type, std::hash<key_type>, std::equal_to<key_type> >;
			//	ToDo: substitute key_type by key
//  			using table_type = std::unordered_map< key, body_type, std::hash<key>, std::equal_to<key> >;
			
		public:
			table() = delete;
			table(table const&);
			
			/**
			 * establich a tables from meta data 
			 */
			table(meta_table_ptr);
			
			/**
			 * Tables are movable
			 */
			table(table&&);

			/**
			 * Desctructor disconnects all listeners
			 */
			virtual ~table();
			
			/**
			 * @return number of elements in the table
			 */
			std::size_t size() const;

			/**
			 * @return meta data
			 */
			meta_table_interface const& meta() const;
			
			/**
			 * Clears the table contents.
			 */
			void clear();
			
			/**
			 * @param key the record key
			 * @param body the body to insert
			 * @return true if the pair was actually inserted.
			 */
			bool insert(key_type const& key, data_type const& data, std::uint64_t generation);

			/**
			 * @param key the record key
			 * @return true if the record was actually deleted
			 */
			bool erase(key_type const& key);
			
			/**
			 * Complexity O(log n) - red black tree.
			 *
			 * @brief simple record lookup
			 * @return true if record was found
			 */
			bool exist(key_type const& key) const;
			
			/**
			 * Complexity O(log n) - red black tree.
			 *
			 * @brief simple record lookup
			 */
			record lookup(key_type const& key) const;
			

			/**
			 * If a matching record was found, the record will be write/exclusive locked.
			 * The modification signal is send in every case, even when old and new values
			 * are equal. (This should be changed in an upcoming version).
			 *
			 * @param key the record key
			 * @param attr a specific attribute of the record body.
			 * @return true if new value was sucessfully written. 
			 */
			bool modify(key_type const& key, attr_t&& attr);

			/**
			 * If a matching record was found, the record will be write/exclusive locked.
			 * The modification signal is send in every case, even when old and new values
			 * are equal. (This should be changed in an upcoming version).
			 *
			 * @param key the record key
			 * @param param a specific parameter of the record body.
			 * @return true if new value was sucessfully written. 
			 */
			bool modify(key_type const& key, param_t&& param);
			
		private:
			/**
			 * This method is potential unsafe, since record data are unlocked
			 * after return from this method. A full table lock is required
			 * to handle this situation in a threadsafe way.
			 * 
			 * @param key the record key
			 * @return a pair consisting of record body and a bool denoting whether the record
			 * was found. If nothing was found, the body is empty (or an undefined state).
			 */
			std::pair<table_type::const_iterator,bool> find(key_type const& key) const;

		private:
			meta_table_ptr meta_;
 			table_type data_;
		};
		
	}	//	store	
	
	namespace traits
	{
		template <>
		struct type_tag<cyng::store::table>
		{
			using type = cyng::store::table;
			using tag =  std::integral_constant<std::size_t, PREDEF_TABLE>;
#if defined(CYNG_LEGACY_MODE_ON)
			const static char name[];
#else
			constexpr static char name[] = "table";
#endif
		};
		
		template <>
		struct reverse_type < PREDEF_TABLE >
		{
			using type = cyng::store::table;
		};
	}
}

namespace std
{
	template<> 
	struct hash<cyng::store::table>
	{
		inline size_t operator()(cyng::store::table const& t) const noexcept
		{
			return std::hash<std::string>{}(t.meta().get_name());
		}
	};
	template<> 
	struct equal_to<cyng::store::table>
	{
		using result_type = bool;
		using first_argument_type = cyng::store::table;
		using second_argument_type = cyng::store::table;
		
		inline bool operator()(cyng::store::table const& t1, cyng::store::table const& t2) const noexcept
		{	
			return t1.meta().get_name() == t2.meta().get_name();
		}
	};	
}

#endif	//	CYNG_STORE_TABLE_H





