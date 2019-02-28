/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_STORE_TABLE_H
#define CYNG_STORE_TABLE_H


#include <cyng/table/meta_interface.h>
#include <cyng/store/publisher.h>
#include <cyng/table/record.h>
#include <cyng/intrinsics/traits.hpp>
#include <cyng/compatibility/async.h>
#include <functional>
#include <unordered_map>

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

		//
		//	forward declaration
		//
		class db;
		class table : public publisher
		{
			friend class db;
			template < typename T >
			friend struct table_functor;
			template < typename T >
			friend struct lock_functor;
			
		public:
			using table_type = std::unordered_map< cyng::table::key_type, cyng::table::body_type, std::hash<cyng::table::key_type>, std::equal_to<cyng::table::key_type> >;
			
		public:
			table() = delete;
			table(table const&);
			
			/**
			 * establich a tables from meta data 
			 */
			table(cyng::table::meta_table_ptr);
			
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
			cyng::table::meta_table_interface const& meta() const;
			cyng::table::meta_table_ptr meta_ptr() const;
			
			/**
			 * Clears the table contents.
			 */
			void clear(boost::uuids::uuid source);
			
			/**
			 * @param key the record key
			 * @param body the body to insert
			 * @return true if the pair was actually inserted.
			 */
			bool insert(cyng::table::key_type const& key
				, cyng::table::data_type const& data
				, std::uint64_t generation
				, boost::uuids::uuid source);

			/**
			 * @param key the record key
			 * @return true if the record was actually deleted
			 */
			bool erase(cyng::table::key_type const& key
				, boost::uuids::uuid source);
			
			/**
			 * Complexity O(log n) - red black tree.
			 *
			 * @brief simple record lookup
			 * @return true if record was found
			 */
			bool exist(cyng::table::key_type const& key) const;
			
			/**
			 * Complexity O(log n) - red black tree.
			 *
			 * @brief simple record lookup
			 */
			cyng::table::record lookup(cyng::table::key_type const& key) const;

			/**
			 * Lookup a single value.
			 *
			 * @param idx index of data column
			 */
			object lookup(cyng::table::key_type const& key, std::size_t idx) const;

			/**
			 * Lookup a single value.
			 *
			 * @param column column name
			 */
			object lookup(cyng::table::key_type const& key, std::string const& column) const;

			/**
			 * If a matching record was found, the record will be write/exclusive locked.
			 * The modification signal is send in every case, even when old and new values
			 * are equal. (This should be changed in an upcoming version).
			 *
			 * @param key the record key
			 * @param attr a specific attribute of the record body.
			 * @return true if new value was sucessfully written. 
			 */
			bool modify(cyng::table::key_type const& key, attr_t&& attr, boost::uuids::uuid source);

			/**
			 * If a matching record was found, the record will be write/exclusive locked.
			 * The modification signal is send in every case, even when old and new values
			 * are equal. (This should be changed in an upcoming version).
			 *
			 * @param key the record key
			 * @param param a specific parameter of the record body.
			 * @return true if new value was sucessfully written. 
			 */
			bool modify(cyng::table::key_type const& key, param_t const& param, boost::uuids::uuid source);
			
			/** @brief Loop over all table entries.
			 * 
			 * Construct for each data row a record and fires a callback.
			 * If callback function return false the loop will be
			 * terminated.
			 *
			 * @return count of invalid/skipped records
			 */
			std::size_t loop(std::function<bool(cyng::table::record const&)> f) const;

			/**
			 * Return record of the first row that matches the specified criteria
			 *
			 * @return the matching record. If no match was found the record is empty
			 */
			cyng::table::record find_first(attr_t&& attr) const;
			cyng::table::record find_first(param_t&& param) const;

			/**
			 * Finds the smallest key in the table and returns the record
			 * of this key. Record is empty if table is empty.
			 */
			cyng::table::record min_record() const;

			/**
			 * Finds the greatest key in the table and returns the record
			 * of this key. Record is empty if table is empty.
			 */
			cyng::table::record max_record() const;

			/**
			 * Convert the table into a vector. Usefull for
			 * serialization to CSV.
			 *
			 * @param col_names if true the first data tuple contains the 
			 * column names
			 */
			vector_t convert(bool col_names) const;

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
			std::pair<table_type::const_iterator, bool> find(cyng::table::key_type const& key) const;

		private:
			cyng::table::meta_table_ptr meta_;
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

	/**
	 * Remove a list of given records
	 */
	std::size_t erase(store::table*, table::key_list_t const& keys, boost::uuids::uuid source);


}

namespace std
{
	template<> 
	struct hash<cyng::store::table>
	{
		size_t operator()(cyng::store::table const& t) const noexcept;
	};

	template<> 
	struct equal_to<cyng::store::table>
	{
		//	pre C++17
		using result_type = bool;
		using first_argument_type = cyng::store::table;
		using second_argument_type = cyng::store::table;
		
		bool operator()(cyng::store::table const& t1, cyng::store::table const& t2) const noexcept;
	};	

	template<>
	struct less<cyng::store::table>
	{
		//	pre C++17
		using result_type = bool;
		using first_argument_type = cyng::store::table;
		using second_argument_type = cyng::store::table;

		bool operator()(cyng::store::table const& t1, cyng::store::table const& t2) const noexcept;
	};
}

#endif	//	CYNG_STORE_TABLE_H





