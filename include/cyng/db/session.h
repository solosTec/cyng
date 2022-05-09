/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#ifndef CYNG_DB_SESSION_H
#define CYNG_DB_SESSION_H


#include <cyng/sql/dialect.h>
#include <cyng/db/connection_types.h>
#include <boost/system/error_code.hpp>


namespace cyng
{
	namespace db 
	{
		class session_interface;
		class statement_interface;
		using session_ptr = std::shared_ptr< session_interface >;
		using statement_ptr = std::shared_ptr< statement_interface >;

		/**	
		 * @brief Facade object of the abstract session interface.
		 *
		 */
		class session
		{
		public:
			session() = delete;
			session(connection_type);

			/**
			 * Sessions can be copied
			 */
			session(session const&);
			
			/**
			 * Sessions can be moved
			 */
			session(session &&);
			
			virtual ~session();
				
			/**
			* assignment
			*/
			session& operator=(session &&);

			/**
			 * Connect to database
			 * @param config a set of parameters containing database specific connection 
			 * data, like account name, password, timeouts, ...
			 */
			std::pair<std::string, bool> connect(param_map_t const& config);

			std::pair<std::string, bool> connect(tuple_t const& config);
			
			/**
			 *	@return true if session is connected, false otherwise.
			 */
			bool is_alive() const;

			/**
			 *	Closes the session.
			 */
			bool close();
			
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
			 *	@note there is one problem open when using ODBC. If an error occurs
			 *	the last error will be read and displayed. This is an ODBC method too
			 *	and therefore the last error result turns to OK.
			 *	Currently there is no solution to solve this shortcoming. Using this
			 *	method in an ODBC environment has limited benefit.
			 *
			 *	@return last error code
			 */
			boost::system::error_code last_error() const;
			
			/**
			 *	@return an object to execute SQL commands
			 */
			statement_ptr create_statement();

			/**
			 * Execute a SQL command
			 * 
			 * @param sql a SQL statement
			 */
			bool execute(std::string const& sql);
			
			/**
			 * @return the SQl dialect specified with 
			 * the config parameters.
			 */
			cyng::sql::dialect get_dialect() const;
			
		private:
			/**
			 *	Replace the existing session type by the new one.
			 */
 			static session_ptr reset(connection_type);
			
		private:
 			session_ptr	connection_;
		};
		
		
		/**
		 * database transaction in RAII style
		 */
		class transaction
		{
		public:
			transaction(session&);
			virtual ~transaction();

			void rollback();

		private:
			session&	db_;
			bool		abort_;
		};


		/**
		 * opens the specified database and return a session instance
		 */
		session create_db_session(object const&);
		session create_db_session(param_map_t const&);


	}	//	db
}	//	cyng

#include <cyng/db/details/statement_interface.h>

#endif	
