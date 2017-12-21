/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SQL_DSL_AGGREGATE_H
#define CYNG_SQL_DSL_AGGREGATE_H

#include <ostream>
#include <cyng/store/meta_interface.h>
#include <cyng/sql/dialect.h>

namespace cyng 
{
	using namespace store;
	namespace sql
	{
		/**
		 * Generates the SQL count() function
		 */
		struct  count
		{
			count(std::size_t index = 0);
			void serialize(std::ostream& os, meta_table_ptr tbl, dialect dia) const;
			const std::size_t index_;
		};

		/**
		 * Generates the SQL sum() function
		 */
		struct  sum
		{
			sum(std::size_t index);
			void serialize(std::ostream& os, meta_table_ptr tbl, dialect dia) const;
			const std::size_t index_;
		};

		/**
		 * Generates the SQL avg() function
		 */
		struct  average
		{
			average(std::size_t index);
			void serialize(std::ostream& os, meta_table_ptr tbl, dialect dia) const;
			const std::size_t index_;
		};

		/**
		 * Generates the SQL max() function
		 */
		struct  maximum
		{
			maximum(std::size_t index);
			void serialize(std::ostream& os, meta_table_ptr tbl, dialect dia) const;
			const std::size_t index_;
		};

		/**
		 * Generates the SQL min() function
		 */
		struct  minimum
		{
			minimum(std::size_t index);
			void serialize(std::ostream& os, meta_table_ptr tbl, dialect dia) const;
			const std::size_t index_;
		};
	}	
}

#endif	//	CYNG_SQL_DSL_AGGREGATE_H




