/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#ifndef CYNG_DB_INTERFACE_STATEMENT_H
#define CYNG_DB_INTERFACE_STATEMENT_H

#include <cyng/db/interface_result.h>
#include <cyng/table/key.hpp>
#include <cyng/table/body.hpp>
#include <cyng/table/meta_interface.h>

namespace cyng
{
	namespace db 
	{
		/*	@brief abstract SQL statement interface.
		 * 
		 * Each implementation of a SQL statement must implement at least that
		 * interface
		 */
		class interface_statement
		{
		public:
			/**
			 * ToDo: change return value to tribool to show that command
			 * execution was successful but no values available.
			 *
			 * @return true if command execution was successful.
			 */
			virtual bool execute() = 0;

			/**
			 *	Immediate execution of an SQL statement
			 */
			virtual bool execute(std::string const& sql) = 0;

			/**
			 * @return first element of the pair contains the number of parameters in an SQL statement.
			 * The second parameter contains true, if prepare statement was successful - otherwise false.
			 */
			virtual std::pair<int, bool> prepare(std::string const& sql) = 0;

			/**
			 * Destroy the prepared statement
			 */
			virtual bool close() = 0;

			//	data binding
			virtual interface_statement& push(object, std::size_t) = 0;
			virtual interface_statement& push(const cyng::table::key_type&, cyng::table::meta_table_ptr ) = 0;
			virtual bool bind(int, object&, std::size_t column_size) = 0;
			virtual bool bind(const cyng::table::key_type&, cyng::table::meta_table_ptr) = 0;
			virtual bool bind(const cyng::table::key_type&, cyng::table::data_type const&, cyng::table::meta_table_ptr) = 0;

			/**
			 * unbound all parameters and reset a prepared statetent
			 */
			virtual void clear() = 0;

			virtual result_ptr get_result() = 0;
		};
		
		typedef std::shared_ptr< interface_statement >	statement_ptr;
		
	}
}

#endif	//	CYNG_DB_INTERFACE_STATEMENT_H
