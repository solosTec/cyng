/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_TABLE_META_HPP
#define CYNG_TABLE_META_HPP

#include <cyng/table/meta_interface.h>
#include <array>
#include <limits>

namespace cyng 
{
	namespace table 
	{
		/**
		 * @tparam KEY_SIZE dimension of key
		 * @tparam BODY_SIZE dimension of data body
		 * @tparam IDX optional index (0 == no index)
		 * 
		 * The index is maintained as an optional index of the data body
		 */
		template < std::size_t KEY_SIZE, std::size_t BODY_SIZE, std::size_t IDX = 0u>
		class meta_table : public meta_table_interface, public std::enable_shared_from_this<meta_table<KEY_SIZE, BODY_SIZE, IDX>>
		{
			using col_names_t = std::array<std::string, KEY_SIZE + BODY_SIZE>;
			using col_types_t = std::array<std::size_t, KEY_SIZE + BODY_SIZE>;
			using col_width_t = std::array<std::size_t, KEY_SIZE + BODY_SIZE>;
			static_assert(IDX < BODY_SIZE, "IDX exceeds BODY_SIZE");
			
		public:
			meta_table(std::string const& name)
			: name_(name)
			, col_names_()
			, col_types_()
			, col_width_()
			{}

			meta_table(std::string const& name, col_names_t const& cols)
			: name_(name)
			, col_names_(cols)
			, col_types_()
			, col_width_()
			{
				BOOST_ASSERT_MSG(std::none_of(cols.begin(), cols.end(), [](std::string const& str) {
					return str.empty();
				}), "column names incomplete");
			}

			meta_table(std::string const& name, col_names_t const& cols, col_types_t const& types)
			: name_(name)
			, col_names_(cols)
			, col_types_(types)
			, col_width_()
			{}

			meta_table(std::string const& name, col_names_t const& cols, col_types_t const& types, col_width_t const& widths)
			: name_(name)
			, col_names_(cols)
			, col_types_(types)
			, col_width_(widths)
			{}

			/**
			 * @return the table name
			 */
			virtual std::string get_name() const override
			{
				return name_;
			}
			
			/**
			 * @return column count (KEY_SIZE + BODY_SIZE)
			 */
			virtual std::size_t size() const override
			{
				return KEY_SIZE + BODY_SIZE;
			}


			/**
			 * Could throw.
			 * 
			 * @return the name of the specified column
			 */
			virtual std::string get_name(std::size_t col) const override 
			{
				return col_names_[col];
			}
			
			/**
			 * Could throw.
			 * 
			 * @return the data type of the specified column
			 */
			virtual std::size_t get_type(std::size_t col) const override 
			{
				return col_types_[col];
			}
			
			/**
			 * Could throw.
			 * 
			 * @return the (display) width of the data type
			 */
			virtual std::size_t get_width(std::size_t col) const override 
			{
				return col_width_[col];
			}
			
			virtual bool check_key(key_type const& key) const override
			{
				return key.size() == KEY_SIZE;
			}
			
			virtual bool check_record(key_type const& key, data_type const& data) const override
			{
				return check_key(key) && check_body(data);
			}
			
			virtual std::pair<std::size_t, bool> get_body_index(std::size_t idx) const override
			{
				return adjust_record_to_body_index(idx);
			}
			
			virtual std::pair<std::size_t, bool> get_body_index(std::string const& name) const override
			{
				//
				//	search column name
				//
				const auto r = get_record_index(name);
				return (r.second)
				? adjust_record_to_body_index(r.first)
				: std::make_pair(std::numeric_limits<std::size_t>::max(), false)
				;
			}
			
			virtual param_t to_param(attr_t const& attr) const override
			{
				return param_t{col_names_[attr.first + KEY_SIZE], attr.second};
			}

			virtual param_map_t to_param_map(attr_t const& attr) const override
			{
				return param_map_t{ to_param(attr) };
			}
			
			virtual param_map_t convert_key(key_type const& key) const override
			{
				param_map_t	params;
				for (std::size_t idx = 0; 
					(idx < key.size()) && (idx < KEY_SIZE);
					++idx)
				{
					params[col_names_[idx]] = key.at(idx);
				}
				return params;
			}
			
			/**
			 * Simply search for a matching entry in the column name array and 
			 * return the index.
			 */
			virtual std::pair<std::ptrdiff_t, bool> get_record_index(std::string const& name) const override
			{
				auto const pos = std::find(col_names_.begin(), col_names_.end(), name);
				return (pos != col_names_.end())
					? std::make_pair(std::distance(col_names_.begin(), pos), true)
					: std::make_pair(std::numeric_limits<std::ptrdiff_t>::max(), false)
					;
			}

			virtual bool is_body(std::size_t idx) const override
			{
				return is_body_index(idx);
			}
			
			virtual bool is_key(std::size_t idx) const override
			{
				return is_key_index(idx);
			}
			
			/**
			 * loop over all columns 
			 */
			virtual void loop(std::function<void(column&&)> cb) const override 
			{
				for (std::size_t pos = 0; pos < KEY_SIZE + BODY_SIZE; ++pos)
				{
					cb(column(pos, get_name(pos), get_type(pos), pos < KEY_SIZE, get_width(pos)));
				}
			}

			virtual void loop_key(std::function<void(column&&)> cb) const
			{
				for (std::size_t pos = 0; pos < KEY_SIZE; ++pos)
				{
					cb(column(pos, get_name(pos), get_type(pos), pos < KEY_SIZE, get_width(pos)));
				}
			}

			virtual void loop_body(std::function<void(column&&)> cb) const
			{
				for (std::size_t pos = KEY_SIZE; pos < KEY_SIZE + BODY_SIZE; ++pos)
				{
					cb(column(pos - KEY_SIZE, get_name(pos), get_type(pos), pos < KEY_SIZE, get_width(pos)));
				}
			}

			/**
			 * @return true if table has a primary key
			 */
			virtual bool has_pk() const override 
			{
				return KEY_SIZE != 0;
			}

			/**
			 * @return true if table maintains an index
			 */
			virtual bool has_index() const override
			{
				return IDX != 0u;
			}

			virtual std::pair<std::size_t, bool> get_index() const override
			{
				return std::make_pair(IDX - 1, has_index());
			}

		private:
			bool check_body(data_type const& data) const
			{
				return data.size() == BODY_SIZE;
			}
			
			std::pair<std::ptrdiff_t, bool> get_key_index(std::string const& name) const
			{
				const auto r = get_record_index(name);
				return (r.second && (r.first < KEY_SIZE))
				? r 
				: std::make_pair(std::numeric_limits<std::ptrdiff_t>::max(), false)
				;
			}
			
			/**
			 * Adjust a given record index to an body index 
			 */
			constexpr std::pair<std::size_t, bool> adjust_record_to_body_index(std::size_t idx) const
			{
				//
				//	fix the offset
				//
				return std::make_pair(idx - KEY_SIZE, is_body_index(idx));
			}
			
			/**
			 * @return true if idx is not in the range of the key indices
			 * and lower than the the record dimension.
			 */
			constexpr bool is_body_index(std::size_t idx) const
			{
				return (idx >= KEY_SIZE) 
					&& (idx < (KEY_SIZE + BODY_SIZE))
					&& (idx != std::numeric_limits<std::size_t>::max())
					;
			}

			constexpr bool is_key_index(std::size_t idx) const
			{
				return (idx < KEY_SIZE) 
					&& (idx != std::numeric_limits<std::size_t>::max())
					;
			}

		private:
			/**
			 * table name
			 */
			const std::string name_;
			const col_names_t col_names_;
			const col_types_t col_types_;
			const col_width_t col_width_;
		};
		
		
		/**
		 * Handy function to create shared pointers of meta tables
		 * 
		 * Example:
		 * @code
		 * auto dbp = store::make_meta_table<1, 2>("name", {"table", "age"});
		 * @endcode
		 */
		template < std::size_t KEY_SIZE, std::size_t BODY_SIZE, std::size_t IDX = 0u>
		meta_table_ptr
		make_meta_table(std::string const& name
		, std::array<std::string, KEY_SIZE + BODY_SIZE> const& cols
		, std::array<std::size_t, KEY_SIZE + BODY_SIZE> const& types = {}
		, std::array<std::size_t, KEY_SIZE + BODY_SIZE> const& widths = {}
		)
		{
			using type = meta_table<KEY_SIZE, BODY_SIZE, IDX>;
			return std::static_pointer_cast<meta_table_interface>(std::make_shared<type>(name, cols, types, widths));
		}
		
	}	//	table	
}

#endif	//	CYNG_TABLE_META_HPP



