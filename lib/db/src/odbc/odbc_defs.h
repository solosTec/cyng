/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#if defined(CYNG_ODBC_INSTALLED)
#ifndef CYNG_DB_ODBC_DEFS_H
#define CYNG_DB_ODBC_DEFS_H

#if defined(_WIN32) || defined(__CYGWIN__)
#include <boost/asio.hpp>	//	windows.h
#endif

#include <sql.h> 
#include <sqlext.h>
#include <odbcinst.h>
#include <boost/config.hpp>

namespace cyng
{
	namespace db
	{	
		namespace odbc
		{
			/**
			*	Global method to check return codes.
			*/
			BOOST_CONSTEXPR inline bool is_ok(SQLRETURN code)	BOOST_NOEXCEPT
			{
				return SQL_SUCCEEDED(code);
			}
			
		}	//	odbc
	}	//	db
}	// cyng

#endif	//	CYNG_DB_ODBC_DEFS_H
#endif
