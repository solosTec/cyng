/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#ifndef CYNG_DB_SQLITE_STATEMENT_H
#define CYNG_DB_SQLITE_STATEMENT_H

#include <cyng.h>

#if defined(CYNG_SQLITE3_CONNECTOR)

#include <sqlite3/sqlite_defs.h>
#include <sqlite3/sqlite_connection.h>
#include <cyng/db/details/statement_interface.h>

namespace cyng	
{
	namespace db	
	{
		namespace sqlite	
		{
			class sqlite_result;
			class statement 
			: public statement_interface
			, public std::enable_shared_from_this<statement>
			{
				friend class sqlite_result;
				
			public:
				typedef std::shared_ptr<statement>	shared_type;
				
			public:
				statement(connection*);
								
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
				virtual bool close();

				//
				//	data binding
				//
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

			private:
				bool is_valid() const;
//				bool bind_null(int index);

				template < type_code C >
				bool bind_value_by_code(object obj, std::size_t column_size);


				operator sqlite3_stmt*();
				
			private:
				/**
				*	Wrapper for sqlite3* pointer
				*/
				connection*		connection_;
				sqlite3_stmt*	stmt_;
				char const*		tail_;
				enum 
				{
					INITIAL,
					READY,
					RUNNING,
					DONE,
				}	state_;
				
				/**
				 * automatic index counter when
				 * pushing bind data.
				 */
				int	bind_counter_;
				
				/**
				 * A list of objects to hold all
				 * static data
				 */
				tuple_t 	static_data_;
				
			};

			statement_ptr statement_factory(connection*);

		}
	}
}

#endif
#endif	//	CYNG_DB_SQLITE_STATEMENT_H
