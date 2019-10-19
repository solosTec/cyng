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
		template < std::size_t KEY_SIZE, std::size_t BODY_SIZE, std::size_t IDX = 0u>
		class meta_table_base
		{
			static_assert(IDX < BODY_SIZE, "IDX exceeds BODY_SIZE");

		public:
			using col_names_t = std::array<std::string, KEY_SIZE + BODY_SIZE>;
			using col_types_t = std::array<std::size_t, KEY_SIZE + BODY_SIZE>;
			using col_width_t = std::array<std::size_t, KEY_SIZE + BODY_SIZE>;


			meta_table_base(std::string const& name)
				: name_(name)
				, col_names_()
				, col_types_()
				, col_width_()
			{}

			meta_table_base(std::string const& name, col_names_t && cols)
				: name_(name)
				, col_names_(std::move(cols))
				, col_types_()
				, col_width_()
			{
				BOOST_ASSERT_MSG(std::none_of(cols.begin(), cols.end(), [](std::string const& str) {
					return str.empty();
				}), "column names incomplete");
			}

			meta_table_base(std::string const& name, col_names_t && cols, col_types_t && types)
				: name_(name)
				, col_names_(std::move(cols))
				, col_types_(types)
				, col_width_()
			{}

			meta_table_base(std::string const& name, col_names_t && cols, col_types_t && types, col_width_t && widths)
				: name_(name)
				, col_names_(std::move(cols))
				, col_types_(types)
				, col_width_(widths)
			{}

		protected:
			/**
			 * table name
			 */
			std::string const name_;
			col_names_t const col_names_;
			col_types_t const col_types_;
			col_width_t const col_width_;
		};

		/**
		 * @tparam KEY_SIZE dimension of key
		 * @tparam BODY_SIZE dimension of data body
		 * @tparam IDX optional index (0 == no index)
		 * 
		 * The index is maintained as an optional index of the data body
		 */
		template < std::size_t KEY_SIZE, std::size_t BODY_SIZE, std::size_t IDX = 0u>
		class meta_table : public meta_table_base<KEY_SIZE, BODY_SIZE, IDX>, public meta_table_interface, public std::enable_shared_from_this<meta_table<KEY_SIZE, BODY_SIZE, IDX>>
		{
			using base = meta_table_base<KEY_SIZE, BODY_SIZE, IDX>;

		public:
			using ext_t = meta_table<KEY_SIZE, BODY_SIZE + 1, IDX>;
			using ext_p = std::shared_ptr<ext_t>;
			
		public:
			meta_table(std::string const& name)
				: base(name)
			{}

			meta_table(std::string const& name, typename base::col_names_t && cols)
				: base(name, std::move(cols))
			{}

			meta_table(std::string const& name, typename base::col_names_t && cols, typename base::col_types_t && types)
				: base(name, std::move(cols), std::move(types))
			{}

			meta_table(std::string const& name, typename base::col_names_t && cols, typename base::col_types_t && types, typename base::col_width_t && widths)
				: base(name, std::move(cols), std::move(types), std::move(widths))
			{}

			/**
			 * @return the table name
			 */
			virtual std::string get_name() const override
			{
				return this->base::name_;
			}
			
			/**
			 * @return column count (KEY_SIZE + BODY_SIZE)
			 */
			virtual std::size_t size() const override
			{
				return KEY_SIZE + BODY_SIZE;
			}

			/**
			 * If col is out of range return value is an empty string.
			 * 
			 * @return the name of the specified column
			 */
			virtual std::string get_name(std::size_t col) const override 
			{
				return (col < size()) 
                    ? this->base::col_names_[col]
                    : ""
                    ;
			}
			
			/**
			 * Could throw.
			 * 
			 * @return the data type of the specified column
			 */
			virtual std::size_t get_type(std::size_t col) const override 
			{
				return this->base::col_types_[col];
			}
			
			virtual std::size_t get_type(std::string name) const  override
			{
				auto const r = get_record_index(name);
				return (r.second) 
					? this->base::col_types_[r.first]
					: TC_NULL
					;
			}

			/**
			 * Could throw.
			 * 
			 * @return the (display) width of the data type
			 */
			virtual std::size_t get_width(std::size_t col) const override 
			{
				return this->base::col_width_[col];
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
				return param_t{this->base::col_names_[attr.first + KEY_SIZE], attr.second};
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
					params[this->base::col_names_[idx]] = key.at(idx);
				}
				return params;
			}
			
			/**
			 * Simply search for a matching entry in the column name array and 
			 * return the index.
			 */
			virtual std::pair<std::size_t, bool> get_record_index(std::string const& name) const override
			{
				auto const pos = std::find(this->base::col_names_.begin(), this->base::col_names_.end(), name);
				if (pos != this->base::col_names_.end()) {
					auto const dist = std::distance(this->base::col_names_.begin(), pos);
					return std::make_pair(static_cast<std::size_t>(dist), true);
				}
				return std::make_pair(std::numeric_limits<std::size_t>::max(), false);
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

			/**
			 * generate extended meta data
			 */
			ext_p get_gen_table() const
			{
				typename ext_t::col_names_t col_names;
				typename ext_t::col_types_t col_types;
				typename ext_t::col_width_t col_width;

				loop([&](column&& col)->void {
					if (col.pk_) {
						col_names[col.pos_] = col.name_;
						col_types[col.pos_] = col.type_;
						col_width[col.pos_] = col.width_;
					}
					else {
						col_names[col.pos_ + 1] = col.name_;
						col_types[col.pos_ + 1] = col.type_;
						col_width[col.pos_ + 1] = col.width_;
					}
					});

				//
				//	additional generation column
				//
				BOOST_ASSERT(col_names[KEY_SIZE].empty());
				col_names[KEY_SIZE] = "gen";
				col_types[KEY_SIZE] = TC_UINT64;
				col_width[KEY_SIZE] = 0u;

				return std::make_shared<ext_t>(get_name(), std::move(col_names), std::move(col_types), std::move(col_width));
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
				, std::array<std::string, KEY_SIZE + BODY_SIZE> && cols = {}
				, std::array<std::size_t, KEY_SIZE + BODY_SIZE> && types = {}
				, std::array<std::size_t, KEY_SIZE + BODY_SIZE> && widths = {}
		)
		{
			using type = meta_table<KEY_SIZE, BODY_SIZE, IDX>;
			return std::static_pointer_cast<meta_table_interface>(std::make_shared<type>(name, std::move(cols), std::move(types), std::move(widths)));
		}
		
		/**
		 * Use the same meta data to generate a meta table with a "gen" column.
		 * The "gen" column is used to store the inherit "generation" of in-memory
		 * tables as explicit SQL column.
		 */
		template < std::size_t KEY_SIZE, std::size_t BODY_SIZE, std::size_t IDX = 0u>
		meta_table_ptr
			make_meta_table_gen(std::string const& name
				, std::array<std::string, KEY_SIZE + BODY_SIZE> && cols = {}
				, std::array<std::size_t, KEY_SIZE + BODY_SIZE> && types = {}
				, std::array<std::size_t, KEY_SIZE + BODY_SIZE> && widths = {}
			)
		{
			using type = meta_table<KEY_SIZE, BODY_SIZE, IDX>;
			using ext_t = typename type::ext_t;

			type tbl(name, std::move(cols), std::move(types), std::move(widths));
			return std::static_pointer_cast<meta_table_interface>(tbl.get_gen_table());
		}

	}	//	table	
}

#endif	//	CYNG_TABLE_META_HPP



