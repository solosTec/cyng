/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#ifndef CYNG_DB_ODBC_STATEMENT_H
#define CYNG_DB_ODBC_STATEMENT_H

#include <cyng.h>

#if defined(CYNG_ODBC_CONNECTOR)

#include <odbc/odbc_defs.h>
#include <odbc/odbc_connection.h>

#include <cyng/db/details/statement_interface.h>

namespace cyng	
{
	namespace db	
	{
		namespace odbc	
		{
			class odbc_result;
			class statement 
			: public statement_interface
			, public std::enable_shared_from_this<statement>
			{
				friend class odbc_result;
				
			public:
				using shared_type = std::shared_ptr<statement>;

			public:
				statement(SQLHDBC h);
				virtual ~statement();
								
				virtual bool execute() override;

				/**
				 *	Immediate execution of an SQL statement
				 */
				virtual bool execute(std::string const& sql) override;

				/**
				 * @return first element of the pair contains the number of parameters in an SQL statement.
				 * The second parameter contains true, if prepare statement was successful - otherwise false.
				 */
				virtual std::pair<int, bool> prepare(std::string const& sql) override;

				/**
				 * Destroy the prepared statement
				 */
				virtual bool close() override;

				//	data binding
				virtual statement_interface* push(object, std::size_t /*= SQL_NTS*/) override;
				virtual statement_interface* push(key_t const&, meta_sql const&) override;
				virtual bool bind(int, object&, std::size_t column_size) override;
				virtual bool bind(key_t const&, data_t const&, meta_sql const&) override;
				virtual bool bind(key_t const&, meta_sql const&) override;

				/**
				 * cleanup a complete table
				 */
				virtual void clear() override;

				/**
				 * @return result set
				 */
				virtual result_ptr get_result() override;
				
				/**
				 * This function returns the number of rows modified, inserted or deleted by the most recently
				 * completed INSERT, UPDATE or DELETE statement on the database connection specified by the only parameter.
				 */
				virtual int changes() override;

			public:
				//	NULL data argument
				static SQLLEN	null_data_;
				static SQLLEN	datetime_data_;
				
			private:
				bool is_valid() const;

				/**
				 *	Guarantee ODBC statement is in proper state.
				 */
				void tidy();

				/**
				 *	Guarantee there is no running fetch and all open
				 *	statements are closed.
				 */
				void stop();

				template < type_code C >
				bool bind_value_by_code(object obj, std::size_t column_size);

				/**
				 * Call SQLFetch
				 */
				bool step();

			private:
				/**
				 *	Wrapper for ODBC statement handle
				 */
				const odbc_statement_handle	stmt_;

				/** 
				 * @see http://msdn.microsoft.com/en-us/library/ms710996(v=vs.85).aspx
				 */
				enum state
				{
					STATE_ERROR,		//!< Unallocated statement. 
					STATE_INITIALIZED,	//!< Allocated statement.
					STATE_PREPARED,		//!< Statement is successful prepared (compare this to prepared_)
					STATE_EXECUTED,		//!< Last execute command was successful
					STATE_POSITIONED,	//!< Reading result set (fetching)
					STATE_TO_CLOSE,		//!< No more data available. 
				}	state_;

				/**
				 * @return true if ODBC statement is in specified state
				 */
				bool is_state(state s) const;
				
				/**
				 * automatic index counter when
				 * pushing bind data (SQLSMALLINT).
				 */
				int bind_counter_;

				/**
				 * A list of objects to hold all
				 * static data
				 */
				tuple_t 	static_data_;
				
			};

			statement_ptr statement_factory(SQLHDBC);

		}
	}
}

#endif
#endif
