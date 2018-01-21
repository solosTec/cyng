/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SQL_DSL_PLACEHOLDER_H
#define CYNG_SQL_DSL_PLACEHOLDER_H

#include <ostream>
#include <cyng/table/meta_interface.h>
#include <cyng/sql/dialect.h>

namespace cyng 
{
	using namespace table;
	namespace sql
	{
		/**
		 * The placeholder cannot detect the appropriate datatype.
		 * The user has to insert special SQL function like julianday(),
		 * datetime(), etc. by itself.
		 */
		struct placeholder
		{
			placeholder();
			void serialize(std::ostream& os, meta_table_ptr tbl, dialect dia) const;
		};
		
		
		/**
		 * Place a ? symbol into the output stream
		 */
		std::ostream& operator<<(std::ostream& os, placeholder const&);

		/**
		 * factory method for placeholders.
		 */
		placeholder make_placeholder();
	}	
}

#endif	//	CYNG_SQL_DSL_PLACEHOLDER_H





