/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_TABLE_RECORD_H
#define CYNG_TABLE_RECORD_H

#include <cyng/table/meta_interface.h>

namespace cyng 
{
	namespace table 
	{
		/**
		 * Define a policy for insert/update operations
		 */
		enum policy {
			POLICY_APPEND,		//!< Only new elements are inserted. Existing elements remain unchanged (insert)
			POLICY_MERGE,		//!< New elements are inserted, existing elements are updated (merge)
			POLICY_SUBSTITUTE,	//!< Only existing elements are updated (update)
		};

		/**
		 * If no string is matching, POLICY_APPEND is assumed
		 */
		policy to_policy(std::string);

		/**
		 * Representation of a table record.
		 * 
		 * Internally data are not stored as records. This is a reference
		 * to the real data.
		 */
		class record 
		{
		public:
			/**
			 * This generates an non-empty record!
			 */
			record(meta_table_ptr);
			record(meta_table_ptr, key_type const&, data_type const&, std::uint64_t);
			record(record const&) = default;
			
			/**
			 * assignment operator
			 */
			record& operator=(record const&);

			/**
			 * Substitute data body. Meta data and key remains
			 * unchanged.
			 * Precondition is that parameter data has the right size.
			 */
			record& operator=(data_type const& data);

			/**
			 * Overwrite all matching body data from the other record.
			 */
			record& read_data(record const& rec);

			/**
			 * @return true is record doesn't contain valid data
			 *
			 * @code
			 meta_.size() != (key().size() + body().size())
			 * @endcode
			 */
			bool empty() const;
			
			/**
			 * read access by index
			 */
			object operator[](std::size_t) const;

			/**
			 * read access name
			 */
			object operator[](std::string) const;

			/**
			 * write access by index
			 */
			void set(std::size_t, object);

			/**
			 * write access by name.
			 *
			 * @return true if specified column exists.
			 */
			bool set(std::string, object);

			/**
			 * @return the key of the record 
			 */
 			key_type const&	key() const;
			
			/**
			 * @return the data part of the record.
			 */
 			data_type const& data() const;
			
			/*
			 * @return generation counter
			 */
			std::uint64_t get_generation() const;

			/**
			 * Convert the record into a tuple. Usefull for
			 * serialization to JSON, XML, you name it.
			 * If no columns names available, the column index is used
			 * as parameter name.
			 */
			tuple_t convert() const;

			/**
			 * Same as convert() but with the option to add more
			 * parameters.
			 *
			 * @param pm addition parameters
			 */
			tuple_t convert(param_map_t const& pm) const;

			/**
			 * Convert the record into a tuple ignoring
			 * all meta data.
			 */
			tuple_t convert_data() const;

			/**
			 * Strip record data of some columns. 
			 * Not existing columns are ignored.
			 * 
			 * @param il list of column names to remove from data body.
			 */
			data_type shrink_data(std::initializer_list<std::string> il) const;

		private:
			object get(std::size_t idx) const;
			
		private:
			meta_table_ptr meta_;
			key_type	key_;
			data_type	data_;
			std::uint64_t generation_;
		};

		/**
		 * Generates an empty record
		 */
		record make_empty_record(meta_table_ptr);

	}	//	table	
}

#endif	//	CYNG_TABLE_RECORD_H




