/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */

#ifndef CYNG_SQL_DSL_PLACEHOLDER_H
#define CYNG_SQL_DSL_PLACEHOLDER_H

#include <cyng/sql/dialect.h>
#include <cyng/store/meta.h>

#include <ostream>

namespace cyng 
{
	namespace sql
	{
		/**
		 * The placeholder cannot detect the appropriate datatype.
		 * The user has to insert special SQL function like julianday(),
		 * datetime(), etc. by itself.
		 */
		struct placeholder
		{
			placeholder(std::string name);

			/**
			 * Place a ? symbol into the output stream
			 */
			std::string to_string(dialect d, meta_sql const&) const;
			std::string const name_;
		};
		
		
		/**
		 * factory method for placeholders.
		 */
		placeholder make_placeholder(std::string name);
	}	
}

#endif	//	CYNG_SQL_DSL_PLACEHOLDER_H





