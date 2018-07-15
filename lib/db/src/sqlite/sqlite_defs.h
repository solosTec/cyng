/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/


#if ! defined CYNG_DB_SQLITE_DEFS_H && (CYNG_SQLITE3_INSTALLED == 1)
#define CYNG_DB_SQLITE_DEFS_H

#include <sqlite3.h>
#include <boost/system/error_code.hpp>
#include <boost/config.hpp>

namespace cyng
{
	namespace db	
	{
		namespace sqlite	
		{
			/**
			*	Global methods to check return codes.
			*/
			BOOST_CONSTEXPR inline bool is_ok(int code) BOOST_NOEXCEPT
			{
				return code == SQLITE_OK;
			}

			BOOST_CONSTEXPR inline bool is_done(int code) BOOST_NOEXCEPT
			{
				return code == SQLITE_DONE;
			}

			BOOST_CONSTEXPR inline bool has_data(int code) BOOST_NOEXCEPT
			{
				return code == SQLITE_ROW;
			}

			inline char const* op_name(int op) BOOST_NOEXCEPT
			{
				switch (op)
				{
				case SQLITE_INSERT:	return "INSERT";
				case SQLITE_DELETE:	return "DELETE";
				case SQLITE_UPDATE:	return "UPDATE";
				default:
					break;
				}
				return "OTHER";
			}

		}	//	sqlite
	}	//	db
}	// cyng

#endif	//	CYNG_DB_SQLITE_DEFS_H
