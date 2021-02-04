/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#ifndef CYNG_SQL_HPP
#define CYNG_SQL_HPP


#include <cyng/sql/dialect.h>
#include <boost/predef.h>

namespace cyng 
{
	namespace sql {

		//
		//	sql commands 
		//
		class select;
		class create;
		class insert;
		class update;
		class remove;


		class select
		{
		public:
			select(dialect);
		private:
			dialect const dialect_;
		};
	}
}

#endif	

