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
		 * Representation of a table record.
		 * 
		 * Internally data are not stored as records. This is a reference
		 * to the real data.
		 */
		class record 
		{
		public:
			record(meta_table_ptr);
			record(meta_table_ptr, key_type const&, data_type const&, std::uint64_t);
			record(record const&);
			
			record& operator=(record const&);

			/**
			 * @return true is record doesn't contain valid data
			 *
			 * @code
			 meta_.size() != (key().size() + body().size())
			 * @endcode
			 */
			bool empty() const;
			
			object operator[](std::size_t) const;
			object operator[](std::string) const;

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

		private:
			object get(std::size_t idx) const;
			
		private:
			meta_table_ptr meta_;
			key_type	key_;
			data_type	data_;
			std::uint64_t generation_;
		};
		
	}	//	table	
}

#endif	//	CYNG_TABLE_RECORD_H




