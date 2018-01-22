/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#if ! defined CYNG_DB_SQLITE_STATEMENT_H && CYNG_SQLITE3_INSTALLED
#define CYNG_DB_SQLITE_STATEMENT_H

#include "sqlite_defs.h"
#include "sqlite_connection.h"
#include <cyng/intrinsics/traits/tag.hpp>
#include <cyng/db/interface_statement.h>
#include <cyng/intrinsics/buffer.h>
#include <cyng/intrinsics/version.h>
#include <cyng/intrinsics/sets.h>
#include <cyng/chrono.h>

namespace cyng	
{
	namespace db	
	{
		namespace sqlite	
		{
			class sqlite_result;
			class statement 
			: public interface_statement
			, public std::enable_shared_from_this<statement>
			{
				friend class sqlite_result;
				
			public:
				typedef std::shared_ptr<statement>	shared_type;
				
			public:
				statement(connection&);
								
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

				//	data binding
				virtual interface_statement& push(object, std::size_t /*= SQL_NTS*/) override;
				virtual interface_statement& push(const cyng::table::key_type&, cyng::table::meta_table_ptr) override;
				virtual bool bind(int, object&, std::size_t column_size) override;
				virtual bool bind(const cyng::table::key_type&, cyng::table::data_type const&, cyng::table::meta_table_ptr) override;
				virtual bool bind(const cyng::table::key_type&, cyng::table::meta_table_ptr) override;

				/**
				 * cleanup a complete table
				 */
				virtual void clear() override;
				
				virtual result_ptr get_result() override;
				
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
				connection&		connection_;
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

			statement_ptr statement_factory(connection&);

		}
	}
}

#endif	//	CYNG_DB_SQLITE_STATEMENT_H
