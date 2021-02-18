/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_STORE_TABLE_H
#define CYNG_STORE_TABLE_H

#include <cyng/store/pub.h>
#include <cyng/store/meta.h>
#include <cyng/store/record.h>
#include <cyng/obj/compare.hpp>

#include <unordered_map>
#include <memory>

namespace cyng {

	class store;
	class table : public pub, public std::enable_shared_from_this<table> {

		//
		//	wants to access the mutex
		//
		friend store;

		using table_t = std::unordered_map< key_t, body, std::hash<key_t>, std::equal_to<key_t> >;

	public:
		table() = delete;

		/**
		 * create a in-memory table from meta info
		 */
		table(meta_store const&);

		virtual ~table();

		/**
		 * @return number of elements in the table
		 */
		std::size_t size() const noexcept;

		/**
		 * Sets the number of buckets to the number needed to accomodate at least count elements
		 * without exceeding maximum load factor and rehashes the container.
		 */
		void reserve(std::size_t);

		/**
		 * @return meta data
		 */
		meta_store const& meta() const noexcept;

		/**
		 * Clears the table contents.
		 */
		void clear(boost::uuids::uuid source);

		/**
		 * Place a new record into the table. Failes if a records
		 * with the same key already exists.
		 *
		 * @param key the record key
		 * @param data the body to insert
		 * @param generation only needed for insert operations
		 * @param source identifier for data source
		 * @return true if the record was actually inserted.
		 */
		bool insert(key_t const& key
			, data_t&& data
			, std::uint64_t generation
			, boost::uuids::uuid source);

		/**
		 * Place a new record into the table. If a records
		 * with the same key already exists it will be replaced.
		 *
		 * @param key the record key
		 * @param data the body to insert
		 * @param generation only needed for insert operations
		 * @param source identifier for data source
		 * @return true if the record was actually inserted or modified
		 */
		bool merge(key_t const& key
			, data_t&& data
			, std::uint64_t generation
			, boost::uuids::uuid source);

		/** @brief search for primary key
		 * 
		 * Complexity O(1) up to O(N)
		 *
		 * @brief simple record lookup
		 * @return true if record was found
		 */
		bool exist(key_t const& key) const;

		/**
		 * Complexity O(1) up to O(N)
		 *
		 * @brief simple record lookup
		 */
		record lookup(key_t const& key) const;

		/**
		 * Lookup by index.
		 * 
		 * @param key search key
		 * @param idx number of index
		 */
		record lookup(key_t const& key, std::size_t idx) const;

		/**
		 * @param key the record key
		 * @return true if the record was actually deleted
		 */
		bool erase(key_t const& key, boost::uuids::uuid source);

		/**
		 * @return true if table is of type auto
		 */
		virtual bool is_auto() const;

		/**
		 * Find a record where the specified data column has the value v.
		 */
		template <typename T>
		record find_first(std::size_t col, T const& v) {

			comparator cmp(v);
			auto pos = std::find_if(std::begin(data_), std::end(data_), [col, &cmp](table_t::value_type const& b) {
				return cmp(b.second.data_.at(col));
			});

			return (pos != std::end(data_)) 
				? record{ meta(), pos->first, pos->second.data_, pos->second.generation_ }
				: record{ meta() }
			;
		}

		/**
		 * Find a record where the specified data column has the value v.
		 */
		template <typename T>
		record find_first(std::string col, T const& v) {
			auto const idx = meta().get_body_index_by_name(col);
			return (idx != std::numeric_limits<std::size_t>::max())
				? find_first(idx, v)
				: record{ meta() }
			;
		}

		/** @brief Loop over all table entries.
		 *
		 * Construct for each data row a record and fires a callback.
		 * If callback function return false the loop will be
		 * terminated.
		 *
		 * @return count of invalid/skipped records
		 */
		std::size_t loop(std::function<bool(record&&, std::size_t)> f) const;

	private:
		bool emplace(key_t const& key
			, data_t&& data
			, std::uint64_t generation
			, bool merge);

		void update(table_t::iterator
			, key_t const& key
			, data_t&& data);

	private:
		meta_store const meta_;
		table_t	data_;

		/**
		 * A sync object for this table
		 */
		mutable std::shared_mutex m_;

	};

	using table_ptr = std::shared_ptr<table>;
}
#endif

