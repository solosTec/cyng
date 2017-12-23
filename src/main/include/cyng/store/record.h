/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_STORE_RECORD_H
#define CYNG_STORE_RECORD_H

#include <cyng/store/meta_interface.h>

namespace cyng 
{
	namespace store 
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
// 			key_type const&	key() const;
			
			/**
			 * @return the data part of the record.
			 */
// 			data_type const& body() const;
			
			/*
			 * @return generation counter
			 */
			std::uint64_t get_generation() const;

		private:
			object get(std::size_t idx) const;
			
		private:
			meta_table_ptr meta_;
			const key_type	key_;
			const data_type	data_;
			const std::uint64_t generation_;
		};
		
	}	//	store	
}

#endif	//	CYNG_STORE_RECORD_H



