/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/


#if ! defined CYNG_DB_SQLITE_CONNECTION_H && defined(CYNG_SQLITE3_INSTALLED)
#define CYNG_DB_SQLITE_CONNECTION_H

#include "sqlite_defs.h"
#include <functional>

namespace cyng
{
	namespace db	
	{
		namespace sqlite	
		{

			//	forward declaration(s):
			class statement;

			/**
			*	Wrapper for sqlite3* pointer
			*/
			class connection	{

				friend class statement;

			public:
				typedef std::function<int(int)>		busy_handler;
				typedef std::function<int()>		commit_handler;
				typedef std::function<void()>		rollback_handler;
				typedef std::function<void(int, char const*, char const*, sqlite3_int64)> update_handler;
				typedef std::function<int(int, char const*, char const*, char const*, char const*)> authorize_handler;

			public:
				connection();
				virtual ~connection();

				bool is_open() const;
				bool open( std::string const& );
				bool close();

				int execute(std::string const& sql);

				int last_error_code() const;
				std::string last_error_msg() const;
				void show_diagnostics() const;

				bool set_busy_timeout(int ms);

				/**
				*	This routine sets a callback function that might be invoked whenever
				*	an attempt is made to open a database table that another thread or
				*	process has locked.
				*/
				void set_busy_handler(busy_handler);

				/**
				*	The sqlite3_commit_hook() interface registers a callback function
				*	to be invoked whenever a transaction is committed.
				*/
				void set_commit_handler(commit_handler);

				/**
				*	The sqlite3_rollback_hook() interface registers a callback function
				*	to be invoked whenever a transaction is rolled back.
				*/
				void set_rollback_handler(rollback_handler);

				/**
				*	The sqlite3_update_hook() interface registers a callback function
				*	with the database connection identified by the first argument
				*	to be invoked whenever a row is updated, inserted or deleted.
				*/
				void set_update_handler(update_handler);

				/**
				*	 The authorizer callback should return SQLITE_OK to allow the action,
				*	SQLITE_IGNORE to disallow the specific action but allow the SQL statement
				*	to continue to be compiled, or SQLITE_DENY to cause the entire SQL statement
				*	to be rejected with an error.
				*/
				void set_authorize_handler(authorize_handler);

				//	unused
				sqlite3_int64 last_insert_rowid() const;
				
				/**
				 * @return true if a transaction is running
				 */
				bool is_transaction() const;

			private:
				operator sqlite3*();

			private:
				/**
				*	Database connection
				*/
				sqlite3*	dbp_;

				//	handler functions
				busy_handler		busy_handler_;
				commit_handler		commit_handler_;
				rollback_handler	rollback_handler_;
				update_handler		update_handler_;
				authorize_handler	authorize_handler_;
			};

		}	//	sqlite
	}	//	db
}	// cyng

#endif	//	CYNG_DB_SQLITE_CONNECTION_H

