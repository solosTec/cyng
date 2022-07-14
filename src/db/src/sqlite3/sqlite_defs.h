/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#ifndef CYNG_DB_SQLITE_DEFS_H
#define CYNG_DB_SQLITE_DEFS_H

#include <cyng.h>

#if defined(CYNG_SQLITE3_CONNECTOR)

#include <sqlite3.h>

namespace cyng
{
	namespace db	
	{
		namespace sqlite	
		{
			/**
			 *	Global methods to check return codes.
			 */
			constexpr bool is_ok(int code) noexcept	{
				return code == SQLITE_OK;
			}

			constexpr bool is_done(int code) noexcept	{
				return code == SQLITE_DONE;
			}

			constexpr bool has_data(int code) noexcept	{
				return code == SQLITE_ROW;
			}

			constexpr char const* op_name(int op) noexcept
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

#endif
#endif
