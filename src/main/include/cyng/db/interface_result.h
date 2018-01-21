/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#ifndef CYNG_DB_INTERFACE_RESULT_H
#define CYNG_DB_INTERFACE_RESULT_H

#include <cyng/object.h>
//#include <cyng/intrinsics/traits/tag.hpp>
#include <memory>
#include <utility>
#include <boost/system/error_code.hpp>

namespace cyng
{
	namespace db 
	{
	
		/*	@brief abstract SQL select interface.
		 *
		 *	Each implementation of a SQL select must implement at least that
		 *	interface
		 */
		class interface_result
		{
		public:
			virtual object get(int, std::size_t, std::size_t /*= SQL_NTS*/ ) = 0;
			virtual std::size_t column_count() = 0;
		};
		
		typedef std::shared_ptr< interface_result >	result_ptr;
		
	}
}

#endif	//	CYNG_DB_INTERFACE_RESULT_H
