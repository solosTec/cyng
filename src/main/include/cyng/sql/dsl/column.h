/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SQL_DSL_COLUMN_H
#define CYNG_SQL_DSL_COLUMN_H

#include <ostream>
#include <cyng/table/meta_interface.h>
#include <cyng/sql/dialect.h>

namespace cyng 
{
	using namespace table;
	namespace sql
	{
		/**
		 * Convert a columns index into a column name.
		 * The index starts with one.
		 */
		struct column
		{
			column(std::size_t index);
			column(std::string name);

			/**
			 * @param lhe don't apply conversion rules on left hand expressions
			 */
			void serialize(std::ostream& os, meta_table_ptr tbl, dialect dia, bool lhe) const;
			
			std::size_t const index_;
			std::string const name_;
			bool is_index_;
		};
		
		 column make_column(std::size_t index);

		/**
		 * Convert a columns index into a distinct column name.
		 * The index starts with one.
		 */
		struct distinct
		{
			distinct(std::size_t index);
			void serialize(std::ostream& os, meta_table_ptr tbl, dialect dia, bool lhe) const;

			const std::size_t index_;
		};

		 distinct make_distinct(std::size_t index);
	}	
}

#endif	//	CYNG_SQL_DSL_COLUMN_H





