/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#if ! defined CYNG_DB_ODBC_SESSION_H && CYNG_ODBC_INSTALLED
#define CYNG_DB_ODBC_SESSION_H

#include "odbc_defs.h"
#include "odbc_connection.h"
#include <cyng/db/interface_statement.h>
#include <cyng/intrinsics/sets.h>
#include <cyng/sql/dialect.h>
#include <utility>
#include <boost/system/error_code.hpp>
			
namespace cyng
{
	namespace db 
	{	
		namespace odbc
		{
			class session	
			{
			public:
				session(SQLUINTEGER = 20, SQLUINTEGER = 60 * 60);
				virtual ~session();
				
				/*
				*	Connect to database 
				* config supports the following parameters:
				* <ul>
				* <li>"connect" (string) -- connect string </li>
				* <li>"connect-timeout" (unsigned int) -- connect timout</li>
				* <li>"login-timeout" (unsigned int) -- login timout</li>
				* </ul>
				*/
				std::pair<std::string, bool> connect(param_map_t const&);
				
				/**
				*	Close connection
				*/
				void close();

				/*
				*	@return true if connection is alive
				*/
				bool is_alive() const;
				
				/**	@brief transaction interface
				*	Starts a transaction.
				*/
				bool begin();
				
				/**	@brief transaction interface
				*	Commits and ends a transaction.
				*/
				bool commit();
				
				/**	@brief transaction interface
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
				/**
				*	Database connection
				*/
				connection	connection_;

				/**
				*	Login timeout is fixed.
				*	Number of seconds to wait for a login request to complete before returning 
				*	to the application. 
				*/
				const SQLUINTEGER	login_timeout_;

				/**
				*	Number of seconds to wait for any request on the connection to complete 
				*	before returning to the application
				*/
				const SQLUINTEGER	connect_timeout_;
				
				cyng::sql::dialect 	sql_dialect_;
				
			};
		}
	}
}

#endif	//	CYNG_DB_ODBC_SESSION_H
