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
#include <boost/fusion/include/make_vector.hpp>

namespace cyng 
{
	namespace store 
	{
		enum class trx_type {
			START,
			COMMIT,
			ROLLBACK
		};

		/**
		 * A store consist of a list of named tables. Each table has a consistent
		 * structure, described in a special structure named meta.
		 * A tables containts records consisting of keys and values.
		 * 
		 * Internally a store is a table of tables. This offers a natural way
		 * to use all table (lock) mechanisms for the database too.
		 *
		 * Each modifying operation requires to specify an event source (UUID),
		 * so the receiver of modification events can identify events
		 * triggered by itself.
		 */
		class db
		{
		public:
			using trx_signal = boost::signals2::signal<void(trx_type)>;

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
			bool create_table(cyng::table::meta_table_ptr);
			
			/**
			 * @return number of tables
			 */
			std::size_t size() const;
			
			/**
			 * generic access on tables
			 */
#if defined(__CPP_SUPPORT_N3915)
			template <class F, typename ...Tbls>
			constexpr decltype(auto)
			access(F&& f, Tbls&& ... tbls)
#else
			template <class F, typename ...Tbls>
			void
			access(F f, Tbls&& ... tbls)
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
				
				//
				//	ToDo: Return if tbl_list contains empty table pointers.
				//	tbl_list is of type: (std::tuple<cyng::store::table *>)
				//
// 				if ((std::tuple_size<decltype(tbl_list)>::value == 1) && (std::get<0>(tbl_list) == nullptr))	return;
				//if ((std::tuple_size<decltype(tbl_list)>::value == 2) && (std::get<1>(tbl_list) == nullptr))	return;
				//if ((std::tuple_size<decltype(tbl_list)>::value == 3) && (std::get<2>(tbl_list) == nullptr))	return;
				//	...

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
#if defined(__CPP_SUPPORT_N3915)
				return cyng::meta::apply(std::forward<F>(f), tbl_list);
#else
				cyng::meta::apply(f, tbl_list);
#endif
				
				//
				//	implicit unlock of all affected tables
				//
			}

			/**
			 * Complexity O(log n) - red black tree.
			 *
			 * @brief simple record lookup
			 */
#if defined(__CPP_SUPPORT_N3915)
			cyng::table::record lookup(std::string table, cyng::table::key_type const& key);
#endif

			
			/**
			 * Select a specific value from a table. The requested will be temporarily
			 * read locked.
			 */
			template <typename ...Args>
			object get_value(std::string table, std::string column, Args&& ... args)
			{
				//
				//	build key
				//
				auto const key = cyng::table::key_generator(std::forward<Args>(args)...);

				object obj;
				access([&](cyng::store::table const* tbl)->void {

					//
					//	search record
					//
					obj = tbl->lookup(key, column);

				}, cyng::store::read_access(table));

				return obj;
			}

			/**
			 * Clears the table contents.
			 *
			 * @param name table name
			 * @param source identifier for data source
			 */
			void clear(std::string const& name, boost::uuids::uuid source);

			/**
			 * @param name table name
			 * @param key the record key
			 * @param data the body to insert
			 * @param generation only needed for insert operations
			 * @param source identifier for data source
			 * @return true if the record was actually inserted.
			 */
			bool insert(std::string const& name
				, cyng::table::key_type const& key
				, cyng::table::data_type const& data
				, std::uint64_t generation
				, boost::uuids::uuid source);

			/**
			 * Place a new record into the table. If a records
			 * with the same key already exists it will be replaced.
			 *
			 * @param name table name
			 * @param key the record key
			 * @param data the body to insert
			 * @param generation only needed for insert operations
			 * @param source identifier for data source
			 * @return true if the record was actually inserted or modified
			 */
			bool merge(std::string const& name
				, cyng::table::key_type const& key
				, cyng::table::data_type&& data
				, std::uint64_t generation
				, boost::uuids::uuid source);

			/**
			 * Update existing record. 
			 *
			 * @param name table name
			 * @param key the record key
			 * @param data the body to insert
			 * @param generation only needed for insert operations
			 * @param source identifier for data source
			 * @return true if the record was actually updated. false if record was not found
			 */
			bool update(std::string const& name
				, cyng::table::key_type const& key
				, cyng::table::data_type&& data
				, std::uint64_t generation
				, boost::uuids::uuid source);

			/**
			 * @param name table name
			 * @param key the record key
			 * @param source identifier for data source
			 * @return true if the record was actually deleted
			 */
			bool erase(std::string const& name, cyng::table::key_type const& key, boost::uuids::uuid source);

			/**
			 * If a matching record was found, the record will be write/exclusive locked.
			 * The modification signal is send in every case, even when old and new values
			 * are equal. (This should be changed in an upcoming version).
			 *
			 * @param name table name
			 * @param key the record key
			 * @param attr a specific attribute of the record body.
			 * @param source identifier for data source
			 * @return true if new value was sucessfully written.
			 */
			bool modify(std::string const& name, cyng::table::key_type const& key, attr_t&& attr, boost::uuids::uuid source);

			/**
			 * If a matching record was found, the record will be write/exclusive locked.
			 * The modification signal is send in every case, even when old and new values
			 * are equal. (This should be changed in an upcoming version).
			 *
			 * @param name table name
			 * @param key the record key
			 * @param param a specific parameter of the record body.
			 * @param source identifier for data source
			 * @return true if new value was sucessfully written.
			 */
			bool modify(std::string const& name, cyng::table::key_type const& key, param_t const& param, boost::uuids::uuid source);


#if defined(__CPP_SUPPORT_N3915)
			template <class F, typename ...Tbls>
			bool multi_modify(std::string const& name, cyng::table::key_type const& key, param_t const& param, boost::uuids::uuid source, F&& f, Tbls&& ... tbls)
#else
			template <class F, typename ...Tbls>
			bool multi_modify(std::string const& name, cyng::table::key_type const& key, param_t const& param, boost::uuids::uuid source, F f, Tbls&& ... tbls)
#endif
			{
				//
				//	Read lock on db.
				//
				cyng::async::shared_lock<cyng::async::shared_mutex> sl(this->m_);

				//
				//	Get the tuple with table pointers. The first element is the table to be changed.
				//
				auto tbl_list = get_table_ptrs(write_access(name), std::forward<Tbls>(tbls)...);

				//
				//	Generate a tuple with the appropriate lock types
				//	read/write lock tables
				//  lock_list is a tuple of boost::shared_lock<boost::shared_mutex> and
				//  boost::unique_lock<boost::shared_mutex>.
				//
				auto lock_list = get_locks(write_access(name), std::forward<Tbls>(tbls)...);

				//
				//	Set the lock for all tables
				//
				cyng::meta::apply(sync_functor(), lock_list);

				//
				//	call function F with unpacked table pointers
				//
#if defined(__CPP_SUPPORT_N3915)
				return cyng::meta::apply(std::forward<F>(f), std::tuple_cat(std::make_tuple(key, param, source), tbl_list));
#else
				return cyng::meta::apply(f, std::tuple_cat(std::make_tuple(key, param, source), tbl_list));
#endif
			}

			/**
			 * @return meta data
			 */
			cyng::table::meta_table_ptr meta(std::string const& name) const;

			/**
			 * Create all connections in one call
			 */
			connections_t get_listener(std::string const& name
				, publisher::insert_signal::slot_type const& isig
				, publisher::remove_signal::slot_type const& rsig
				, publisher::clear_signal::slot_type const& csig
				, publisher::modify_signal::slot_type const& msig);

			void disconnect(std::string const& name);

			/**
			 * Connect to insert record slot
			 */
			boost::signals2::connection get_insert_listener(std::string const& name, const publisher::insert_signal::slot_type&);

			/**
			 * Connect to modify record slot
			 */
			boost::signals2::connection get_modify_listener(std::string const& name, const publisher::modify_signal::slot_type&);

			/**
			 * @return The number of all connected slots of specified table
			 */
			std::size_t num_all_slots(std::string const& name) const;
			std::size_t num_all_slots() const;

			/**
			 * @return size of table
			 */
			std::size_t size(std::string const& name) const;

			/**
			 * manage table state
			 */
			bool set_state(std::string const& name, std::uint32_t);
			std::uint32_t get_state(std::string const& name);

			/**
			 * Connect to transaction slot
			 */
			boost::signals2::connection get_trx_listener(const trx_signal::slot_type&);

			/**
			 * @return The number of slots connected to the trx signal
			 */
			std::size_t num_trx_slots() const;

			/**
			 * set transaction state
			 */
			void set_trx_state(trx_type);

			/**
			 * remove all trx listener
			 */
			void disconnect_trx();
			

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
			
		private:
			/**
			 * contains all tables of the database 
			 */
			table tables_;
			
			/**
			 * A sync object for this database
			 */
            mutable mutex_t m_;

			/**
			 * transaction signal
			 */
			trx_signal		trx_signal_;

		};
		
	}	//	store	
	
	namespace traits
	{
		template <>
		struct type_tag<cyng::store::db>
		{
			using type = cyng::store::db;
			using tag =  std::integral_constant<std::size_t, 
#if defined(__CPP_SUPPORT_N2347)
				static_cast<std::size_t>(traits::predef_type_code::PREDEF_DB)
#else
				PREDEF_DB
#endif
			>;

#if defined(__CPP_SUPPORT_N2235)
			constexpr static char name[] = "db";
#else
			const static char name[];
#endif
		};
		
		template <>
		struct reverse_type < 
#if defined(__CPP_SUPPORT_N2347)
			static_cast<std::size_t>(traits::predef_type_code::PREDEF_DB)
#else
			PREDEF_DB 
#endif
		>
		{
			using type = cyng::store::db;
		};
		
	}
	
}

#endif	//	CYNG_STORE_DB_H






