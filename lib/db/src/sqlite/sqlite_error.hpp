/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/


#if ! defined CYNG_DB_SQLITE_ERROR_HPP && defined(CYNG_SQLITE3_INSTALLED)
#define CYNG_DB_SQLITE_ERROR_HPP

#include <boost/system/error_code.hpp>
			
namespace cyng
{
	namespace db 
	{	
		namespace sqlite	
		{
			namespace error	
			{
				enum sqlite_errors	{
					SQLite_OK,			//!<	Successful result
					SQLite_ERROR,		//!<	SQL error or missing database
					SQLite_INTERNAL,	//!<	Internal logic error in SQLite
					SQLite_PERM,		//!<	Access permission denied
					SQLite_ABORT,		//!<	Callback routine requested an abort
					SQLite_BUSY,		//!<	The database file is locked
					SQLite_LOCKED,		//!<	A table in the database is locked
					SQLite_NOMEM,		//!<	A malloc() failed
					SQLite_READONLY,	//!<	Attempt to write a readonly database
					SQLite_INTERRUPT,	//!<	Operation terminated by sqlite3_interrupt()
					SQLite_IOERR,		//!<	Some kind of disk I/O error occurred
					SQLite_CORRUPT,		//!<	The database disk image is malformed
					SQLite_NOTFOUND,	//!<	NOT USED. Table or record not found 
					SQLite_FULL,		//!<	Insertion failed because database is full 
					SQLite_CANTOPEN,	//!<	Unable to open the database file 
					SQLite_PROTOCOL,	//!<	Database lock protocol error 
					SQLite_EMPTY,		//!<	Database is empty 
					SQLite_SCHEMA,		//!<	The database schema changed 
					SQLite_TOOBIG,		//!<	String or BLOB exceeds size limit 
					SQLite_CONSTRAINT,	//!<	Abort due to constraint violation 
					SQLite_MISMATCH,	//!<	Data type mismatch 
					SQLite_MISUSE,		//!<	Library used incorrectly 
					SQLite_NOLFS,		//!<	Uses OS features not supported on host 
					SQLite_AUTH,		//!<	Authorization denied 
					SQLite_FORMAT,		//!<	Auxiliary database format error 
					SQLite_RANGE,		//!<	2nd parameter to sqlite3_bind out of range 
					SQLite_NOTADB,		//!<	File opened that is not a database file 
					SQLite_ROW,			//!<	sqlite3_step() has another row ready 
					SQLite_DONE,		//!<	sqlite3_step() has finished executing 
				};

				//	SQLite error
				extern
					const boost::system::error_category& get_sqlite_category();

				//	The identity of an error_category-derived object is determined by its address.
				static const boost::system::error_category& sqlite_category
					= get_sqlite_category();

			}	//	error
		}	//	sqlite
	}	//	db
}	// cyng

#include <boost/type_traits/integral_constant.hpp>	//	provides true_type

namespace boost		{
	namespace system	{

		template<>
		struct is_error_code_enum<cyng::db::sqlite::error::sqlite_errors>
			: true_type{};


	}	//	namespace system
}	//	namespace boost

namespace cyng	{
	namespace db	{
		namespace sqlite	{
			namespace error	{

				inline boost::system::error_code make_error_code(int e)	{
					return boost::system::error_code(
						static_cast<int>(e), get_sqlite_category());
				}

				//	implementation see error.cpp
				const boost::system::error_category& get_sqlite_category();

			}	//	error
		}	//	sqlite
	}	//	db
}	// cyng


#endif	//	CYNG_DB_SQLITE_ERROR_HPP
