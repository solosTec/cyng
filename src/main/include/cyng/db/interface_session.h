/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#ifndef CYNG_DB_INTERFACE_SESSION_H
#define CYNG_DB_INTERFACE_SESSION_H

#include <cyng/db/interface_statement.h>
#include <cyng/intrinsics/sets.h>
#include <cyng/sql/dialect.h>
#include <boost/system/error_code.hpp>

namespace cyng
{
	namespace db 
	{
		/*	@brief abstract session interface.
		*
		*	Each implementation of a database session must implement at least that
		*	interface
		*/
		class interface_session
		{
		public:
			/**
			*	Connect to database
			* 
			* @param config a set of parameters containing database specific connection 
			* data, like account name, password, timeouts, ...
			*/
			virtual std::pair<std::string, bool> connect(param_map_t const& config) = 0;

			/**
			*	@return true if session is connected, false otherwise.
			*/
			virtual bool is_alive() const = 0;

			/**
			*	Closes the session.
			*/
			virtual void close() = 0;

			/**	@brief transaction interface
			*	Starts a transaction.
			*/
			virtual bool begin() = 0;

			/**	@brief transaction interface
			*	Commits and ends a transaction.
			*/
			virtual bool commit() = 0;

			/**	@brief transaction interface
			*	Rolls back and ends a transaction.
			*/
			virtual bool rollback() = 0;

			/**
			*	Returns true if a transaction is in progress, false otherwise.
			*/
			virtual bool is_transaction() const = 0;

			/**
			*	@return last error code
			*/
			virtual boost::system::error_code last_error() const = 0;
				
			/**
			*	@return an object to execute SQL commands
			*/
			virtual statement_ptr create_statement() = 0;

			/**
			 * Execute a SQL command
			 */
			virtual bool execute(std::string const&) = 0;
			
			/**
			 * @return the SQl dialect specified with 
			 * the config parameters.
			 */
			virtual cyng::sql::dialect get_dialect() const = 0;
			
		};
		
		typedef std::shared_ptr< interface_session >	session_ptr;
		
	}
}

#endif	//	CYNG_DB_INTERFACE_SESSION_H
