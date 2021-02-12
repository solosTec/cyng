/*
* The MIT License (MIT)
*
* Copyright (c) 2021 Sylko Olzscher
*
*/

#ifndef CYNG_DB_STATEMENT_INTERFACE_H
#define CYNG_DB_STATEMENT_INTERFACE_H

#include <cyng/obj/object.h>
#include <cyng/store/meta.h>
#include <cyng/store/key.hpp>
//#include <cyng/db/result_interface.h>
//#include <cyng/table/key.hpp>
//#include <cyng/table/body.hpp>
//#include <cyng/table/meta_interface.h>

namespace cyng
{
	namespace db 
	{
		class result_interface;
		using result_ptr =  std::shared_ptr< result_interface >;

		/*	@brief abstract SQL statement interface.
		 * 
		 * Each implementation of a SQL statement must implement at least that
		 * interface
		 */
		class statement_interface
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
			virtual statement_interface* push(object, std::size_t) = 0;
			virtual statement_interface* push(key_t const&, meta_sql const&) = 0;
			virtual bool bind(int, object&, std::size_t column_size) = 0;
			virtual bool bind(key_t const&, meta_sql const&) = 0;
			virtual bool bind(key_t const&, data_t const&, meta_sql const&) = 0;

			/**
			 * unbound all parameters and reset a prepared statetent
			 */
			virtual void clear() = 0;

			/**
			 * @return result set
			 */
			virtual result_ptr get_result() = 0;

			/**
			 * This function returns the number of rows modified, inserted or deleted by the most recently 
			 * completed INSERT, UPDATE or DELETE statement on the database connection specified by the only parameter.
			 */
			virtual int changes() = 0;

		};
		
		using statement_ptr = std::shared_ptr< statement_interface >;
		
	}
}

#endif	
