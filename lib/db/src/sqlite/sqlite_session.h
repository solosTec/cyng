/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#if ! defined CYNG_DB_SQLITE_SESSION_H && CYNG_SQLITE3_INSTALLED
#define CYNG_DB_SQLITE_SESSION_H

#include "sqlite_defs.h"
#include "sqlite_connection.h"
#include <cyng/db/interface_statement.h>
#include <cyng/intrinsics/sets.h>
#include <cyng/sql/dialect.h>

namespace cyng	
{
	namespace db	
	{
		namespace sqlite	
		{
			class session	
			{

			public:
				session(int = 60 * 60);

				virtual ~session();

				/*
				  * Connect to database .
				  * 
				  * config supports the following parameters:
				  * <ul>
				  * <li>"file-name" (string)</li>
				  * <li>"busy-timeout" (int)</li>
				  * </ul>
				  */
				std::pair<std::string, bool> connect(param_map_t const& config);

				/**
				 *	Close connection
				 */
				bool close();

				/**
				 *	@return true if connection is alive
				 */
				bool is_alive() const;

				/**
				 *	Starts a transaction.
				 */
				bool begin();

				/**
				 *	Commits and ends a transaction.
				 */
				bool commit();

				/**
				 *	Rolls back and ends a transaction.
				 */
				bool rollback();

				/**
				 *	Returns true if a transaction is in progress, false otherwise.
				 */
				bool is_transaction() const;

				/**
				 *	@return last error code
				 */
				boost::system::error_code last_error() const;

				/**
				 *	@return an object to execute SQL commands
				 */
				statement_ptr create_statement();

				/**
				 * Execute a SQL command
				 */
				bool execute(std::string const&);

				/**
				 * @return the SQl dialect specified with 
				 * the config parameters.
				 */
				cyng::sql::dialect get_dialect() const;
				
			private:
				void release_memory();

				void busy_monitor(int counter);

			private:
				/**
				 *	Wrapper for sqlite3* pointer
				 */
				connection	connection_;

				/**
				 *	Number of milliseconds to wait for any request on the connection to complete 
				 *	before returning to the application
				 */
				const int	busy_timeout_;

			};

		}	//	sqlite
	}	//	db
}	// cyng

#endif	//	CYNG_DB_SQLITE_SESSION_H
