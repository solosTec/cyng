/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#ifndef CYNG_DB_SQLITE_SESSION_H
#define CYNG_DB_SQLITE_SESSION_H

#include <cyng.h>

#if defined(CYNG_SQLITE3_CONNECTOR)

#include <sqlite3/sqlite_defs.h>
//#include "sqlite_connection.h"
#include <cyng/db/details/session_interface.h>
//#include <cyng/db/details/statement_interface.h>
#include <cyng/obj/intrinsics/container.h>
#include <cyng/sql/dialect.h>

#include <memory>

namespace cyng	
{
	namespace db	
	{
		namespace sqlite	
		{
			class connection;
			class session : public session_interface
			{

			public:
				session(int busy_timeout = 60 * 60);

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
				virtual std::pair<std::string, bool> connect(param_map_t const& config) override;

				/**
				 *	@return true if connection is alive
				 */
				virtual bool is_alive() const override;

				/**
				 *	Close connection
				 */
				virtual bool close() override;

				/**
				 *	Starts a transaction.
				 */
				virtual bool begin() override;

				/**
				 *	Commits and ends a transaction.
				 */
				virtual bool commit() override;

				/**
				 *	Rolls back and ends a transaction.
				 */
				virtual bool rollback() override;

				/**
				 *	Returns true if a transaction is in progress, false otherwise.
				 */
				virtual bool is_transaction() const override;

				/**
				 *	@return last error code
				 */
				virtual boost::system::error_code last_error() const override;

				/**
				 *	@return an object to execute SQL commands
				 */
				virtual statement_ptr create_statement() override;

				/**
				 * Execute a SQL command
				 */
				virtual bool execute(std::string const&) override;

				/**
				 * @return the SQl dialect specified with 
				 * the config parameters.
				 */
				virtual cyng::sql::dialect get_dialect() const override;
				
			private:
				void release_memory();

				void busy_monitor(int counter);

			private:
				/**
				 *	Wrapper for sqlite3* pointer
				 */
				std::unique_ptr<connection>	connection_;

				/**
				 *	Number of milliseconds to wait for any request on the connection to complete 
				 *	before returning to the application
				 */
				int	const busy_timeout_;

			};

		}	//	sqlite
	}	//	db
}	// cyng

#endif
#endif	