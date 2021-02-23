/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_STORE_DB_H
#define CYNG_STORE_DB_H

#include <cyng/store/auto_table.h>
#include <cyng/meta.hpp>
#include <cyng/io/ostream.h>

#include <functional>
#include <shared_mutex>
#include <map>
#include <tuple>
#include <type_traits>


namespace cyng {

	namespace access {
		struct read {
			read() = delete;
			read(read const&) = default;
			read(std::string);
			std::string const name_;
		};
		struct write {
			write() = delete;
			write(write const&) = default;
			write(std::string);
			std::string const name_;
		};

		/**
		 * calculate table pointer type
		 */
		template <typename T>
		struct get_ptr_type {
			using type = void;
			//static_assert(false, "unknown pointer type");
		};
		template <>
		struct get_ptr_type< read > {
			//	table const*
			using type = typename std::add_pointer< typename std::add_const< table >::type >::type;
		};
		template <>
		struct get_ptr_type< write > {
			//	table *
			using type = typename std::add_pointer< table >::type;
		};

		/**
		 * calculate lock type
		 */
		template <typename T>
		struct get_lock_type {
			using type = void;
			//static_assert(false, "unknown lock type");
		};
		template <>
		struct get_lock_type< read > {
			using type = std::shared_lock<std::shared_mutex>;
		};
		template <>
		struct get_lock_type< write > {
			using type = std::unique_lock<std::shared_mutex>;
		};
	}

	class store
	{
	public:
		store();

		/**
		 * create and insert a new table
		 * @return true if table was actually inserted.
		 */
		bool create_table(meta_store const&);
		bool create_auto_table(meta_store const&, key_t const& start_key, auto_table::auto_key_f f);

		/**
		 * @return number of tables
		 */
		std::size_t size() const noexcept;

		/**
		 * safe access to multiple tables at once
		 */
		template <typename F, typename ...Tbls>
		constexpr decltype(auto)
		access(F&& f, Tbls&& ... tbls) /*-> typename std::invoke_result<F>::type*/ {

			//
			//	read lock on database
			//
			std::shared_lock<std::shared_mutex> sl(m_);

			//
			//	transform the Tbls... into a tuple of lock types
			//
			auto locks = get_locks(tbls...);
			static_assert(std::tuple_size<decltype(locks)>::value == sizeof...(Tbls), "wrong lock count");

			//
			//	Set the lock for all tables
			//	std::lock() requires at least two parameters
			//
			if constexpr (sizeof...(Tbls) == 1) {
				std::get<0>(locks).lock();
			}
			else {

				std::apply([](auto &&... args) {
					//std::cout << "lock(" << sizeof...(args) << ")" << std::endl;
					std::lock(args...);
					}, locks);
			}

			//
			//	get a list of all requested tables
			//
			auto ts = get_table_ptrs(tbls...);

			//
			//	Call function f
			//
			return std::apply([f](auto &&... args) { f(args...); }, std::move(ts));
 
			//
			//	implicit unlock of all affected tables
			//	
			//	to explicit unlock:
			//	using T = tmp::transform_t<std::tuple, access::get_lock_type, Tbls ...>;
			//	locks.~T();
			//

		}

		/**
		 * Clears the table contents.
		 *
		 * @param name table name
		 * @param source identifier for data source
		 */
		void clear(std::string const& name, boost::uuids::uuid source);

		/**
		 * Complexity O(1) up to O(N)
		 *
		 * @brief simple record lookup
		 */
		//record lookup(std::string const& name, key_t const& key);

		/**
		 * Select a specific value from a table. The requested will be temporarily
		 * read locked.
		 */
		template <typename ...Args>
		object get_object(std::string name, std::string column, Args&& ... args)
		{
			//
			//	build key
			//
			auto const key = key_generator(std::forward<Args>(args)...);

			object obj;
			access([&](table const* tbl) {

				//
				//	search record
				//
				obj = tbl->lookup(key).at(column);

			}, access::read(name));

			return obj;
		}

	private:

		template <typename ...Tbls>
		auto get_locks(Tbls... tbls) -> tmp::transform_t<std::tuple, access::get_lock_type, Tbls ...> {
			return { get_lock<Tbls>(tbls)... };
		}

		template <typename T>
		auto get_lock(T tbl) -> typename access::get_lock_type< T >::type {

			//
			//	get return type
			//
			using type = typename access::get_lock_type< T >::type;

			auto pos = tables_.find(tbl.name_);
			BOOST_ASSERT(pos != tables_.end());
			return (pos != tables_.end())
				? type(pos->second->m_, std::defer_lock)
				: type()
				;

		}

		/**
		 * @return tuple of table pointers
		 */
		template <typename ...Tbls>
		auto get_table_ptrs(Tbls... tbls) -> tmp::transform_t<std::tuple, access::get_ptr_type, Tbls ...> {
			return { get_tbl_ptr<Tbls>(tbls)... };
		}

		/**
		 * Find the specified table by name and return a pointer with the 
		 * appropriate type. 
		 * 
		 * @return nullptr if table wasn't found
		 */
		template <typename T>
		auto get_tbl_ptr(T tbl) -> typename access::get_ptr_type< T >::type {
			auto pos = tables_.find(tbl.name_);
			return (pos != tables_.end())
				? pos->second.get()
				: nullptr
				;
		}

	private:
		/**
		 * Contains all tables.
		 * Using a shared pointer allows to use different derivated classes
		 * from table and to store in the same map.
		 */
		std::map<std::string, table_ptr>	tables_;

		/**
		 * A sync object for this database
		 */
		mutable std::shared_mutex m_;

	};
}
#endif

