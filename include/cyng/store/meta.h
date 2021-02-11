/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_STORE_META_H
#define CYNG_STORE_META_H

#include <cyng/store/key.hpp>
#include <cyng/obj/tag.hpp>

#include <string>
#include <memory>
#include <functional>
#include <limits>

#include <boost/assert.hpp>
#include <boost/algorithm/string.hpp>

namespace cyng {

	struct column
	{
		column(std::string const&, type_code);
		column(column const&) = default;

		std::string const name_;
		type_code const type_;	//!<	type_code
	};

	struct column_sql : public column
	{
		column_sql(column const&, std::size_t = 0u);
		column_sql(std::string const&, type_code, std::size_t = 0u);
		column_sql(column_sql const&) = default;

		std::size_t const width_;	//	sql only
	};

	/**
	 * Primary key, secondary key (index) is defined as list of references into the vector of columns.
	 * Multiple keys are allowed. All secondary keys are unique too.
	 */
	using pk_t = std::vector<std::size_t>;
	using sk_t = std::vector<pk_t>;

	template <typename COLUMN = column>
	class meta
	{
	public:
		meta(std::string name)
			: name_(name)
			, columns_()
			, pk_(0)
			, sk_()
		{}

		template <typename I>
		meta(std::string name, I begin, I end, std::size_t pk)
			: name_(name)
			, columns_(begin, end)
			, pk_(pk)
			, sk_()
		{
			BOOST_ASSERT_MSG(pk < columns_.size(), "key size exceeded");
		}

		meta(std::string name, std::initializer_list<COLUMN> cols, std::size_t pk)
			: meta(name, cols.begin(), cols.end(), pk)	//	forward
		{}

		std::string const& get_name() const noexcept {
			return name_;
		}
		std::size_t key_size() const noexcept {
			return pk_;
		}
		std::size_t body_size() const noexcept {
			return size() - key_size();
		}
		std::size_t size() const noexcept {
			return columns_.size();
		}

		std::string get_name(std::size_t col) const {
			return (col < columns_.size())
				? columns_.at(col).name_
				: std::to_string(col)
				;
		}

		COLUMN const& get_column(std::size_t col) const noexcept {
			return (col < columns_.size())
				? columns_.at(col)
				: null_
				;
		}

		/**
		 * @return the absolut index of the column
		 */
		std::size_t const get_index_by_name(std::string name) const noexcept {
			auto const pos = std::find_if(std::begin(columns_), std::end(columns_), [name](COLUMN const& col) {
				return boost::algorithm::equals(name, col.name_);
				});

			return (pos != std::end(columns_))
				? std::distance(std::begin(columns_), pos)
				: std::numeric_limits<std::size_t>::max()
				;
		}

		std::size_t const get_body_index_by_name(std::string name) const noexcept {
			std::size_t const idx = get_index_by_name(name);
			return (idx != std::numeric_limits<std::size_t>::max())
				? idx - key_size()
				: idx
				;
		}

		/**
		 * @return true if specified column is part of primary key
		 */
		bool is_pk(std::size_t col) const noexcept {
			return col < pk_;
		}

		/**
		 * @return true if specified column is part of primary key
		 */
		bool is_pk(std::string name) const noexcept {
			return is_pk(get_index_by_name(name));
		}

		/**
		 * @return true table has a primary key
		 */
		bool has_pk() const noexcept {
			return pk_ != 0;
		}

		/**
		 * @return true table has seconday key(s)
		 */
		bool has_sk() const noexcept {
			return !sk_.empty();
		}

		bool check_integrity(key_t const& key, data_t const& data) const noexcept {
			return check_key_integrity(key) && check_data_integrity(data);
		}

		bool check_key_integrity(key_t const& key) const noexcept {
			return key.size() == key_size();
		}

		bool check_data_integrity(data_t const& data) const noexcept {
			return data.size() == body_size();
		}

		/**
		 * loop over all columns
		 */
		void loop(std::function<void(std::size_t, COLUMN const&, bool)> cb) const {
			std::size_t idx{ 0 };
			for (auto const& col : columns_) {
				cb(
					(idx < pk_) ? idx : (idx - pk_),
					col, 
					idx < pk_);	//	pk starts at 0
				++idx;
			}
		}

	private:
		std::string const name_;
		std::vector<COLUMN>	const columns_;

		/** @brief size of the primary key
		 * 
		 * pk starts always at 0. 
		 * If pk_ is 0 then no primary key is used.
		 */
		std::size_t const pk_;	
		sk_t const sk_;

		/**
		 * column not found
		 */
		static COLUMN null_;
	};

	//
	//	initialize static
	//
	template <typename COLUMN>
	COLUMN meta<COLUMN>::null_("", std::numeric_limits<std::size_t>::max(), 0);
		
	/**
	 * in-memory table
	 */
	using meta_store = meta<column>;

	/**
	 * SQL table with width info
	 */
	using meta_sql = meta<column_sql>;

	/**
	 * convert an in-memory table to a SQL table
	 */
	meta_sql to_sql(meta_store const&, std::vector<std::size_t> const&  width);

	/**
	 * convert an in-memory table to a SQL table
	 */
	meta_store to_mem(meta_sql const&);
}
#endif

