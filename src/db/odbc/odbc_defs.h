/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#ifndef CYNG_DB_ODBC_DEFS_H
#define CYNG_DB_ODBC_DEFS_H

#include <cyng.h>

#if defined(CYNG_ODBC_CONNECTOR)

//#if defined(_WIN32) || defined(__CYGWIN__)
//#include <boost/asio.hpp>	//	windows.h
//#endif

#include <sql.h> 
#include <sqlext.h>
#include <odbcinst.h>

namespace cyng
{
	namespace db
	{	
		namespace odbc
		{
			/**
			 *	Global method to check return codes.
			 */
			constexpr bool is_ok(SQLRETURN code) noexcept	{
				return SQL_SUCCEEDED(code);
			}		
		}	//	odbc
	}	//	db
}	// cyng

#endif
#endif
