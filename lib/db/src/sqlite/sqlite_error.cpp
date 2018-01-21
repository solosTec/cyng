/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/


#include "sqlite_error.hpp"

namespace cyng	
{
	namespace db	
	{
		namespace sqlite	
		{
			namespace error	
			{
				namespace detail 
				{
					class sqlite_errors_category_impl
						: public boost::system::error_category
					{
					public:
						//	return a string identifying the category.
						virtual const char* name() const BOOST_SYSTEM_NOEXCEPT 
						{
							return "SQLite";
						}
						//	Converts an error value into a string that describes the error.
						virtual std::string message(int ev) const	
						{
							switch (ev)	{
							case SQLite_OK:			return "OK";
							case SQLite_ERROR:		return "SQL error or missing database";
							case SQLite_INTERNAL:	return "Internal logic error in SQLite";
							case SQLite_PERM:		return "Access permission denied";
							case SQLite_ABORT:		return "Callback routine requested an abort";
							case SQLite_BUSY:		return "The database file is locked";
							case SQLite_LOCKED:		return "A table in the database is locked";
							case SQLite_NOMEM:		return "A malloc() failed";
							case SQLite_READONLY:	return "Attempt to write a readonly database";
							case SQLite_INTERRUPT:	return "Operation terminated by sqlite3_interrupt()";
							case SQLite_IOERR:		return "Some kind of disk I/O error occurred";
							case SQLite_CORRUPT:	return "The database disk image is malformed";
							case SQLite_NOTFOUND:	return "NOT USED. Table or record not found";
							case SQLite_FULL:		return "Insertion failed because database is full";
							case SQLite_CANTOPEN:	return "Unable to open the database file";
							case SQLite_PROTOCOL:	return "Database lock protocol error";
							case SQLite_EMPTY:		return "Database is empty";
							case SQLite_SCHEMA:		return "The database schema changed";
							case SQLite_TOOBIG:		return "String or BLOB exceeds size limit";
							case SQLite_CONSTRAINT:	return "Abort due to constraint violation";
							case SQLite_MISMATCH:	return "Data type mismatch";
							case SQLite_MISUSE:		return "Library used incorrectly";
							case SQLite_NOLFS:		return "Uses OS features not supported on host";
							case SQLite_AUTH:		return "Authorization denied";
							case SQLite_FORMAT:		return "Auxiliary database format error";
							case SQLite_RANGE:		return "2nd parameter to sqlite3_bind out of range";
							case SQLite_NOTADB:		return "File opened that is not a database file";
							case SQLite_ROW:		return "sqlite3_step() has another row ready";
							case SQLite_DONE:		return "sqlite3_step() has finished executing";
							default:
								break;
							}
							return "[error]";
						}
					};
				}	//	detail

				const boost::system::error_category&
					get_sqlite_category()	
					{
						static detail::sqlite_errors_category_impl instance;
						return instance;
					}
	
			}	//	error
		}	//	sqlite
	}	//	db
}	// cyng
