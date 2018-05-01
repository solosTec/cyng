/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#if ! defined CYNG_DB_ODBC_STATEMENT_H && CYNG_ODBC_INSTALLED
#define CYNG_DB_ODBC_STATEMENT_H

#include "odbc_defs.h"
#include "odbc_connection.h"
#include <cyng/db/interface_statement.h>
#include <cyng/intrinsics/buffer.h>
#include <cyng/intrinsics/version.h>
#include <cyng/intrinsics/sets.h>
#include <cyng/chrono.h>
//#include <noddy/m2m/intrinsics/obis.h>
//#include <noddy/m2m/intrinsics/ctrl_address.h>

namespace cyng	
{
	namespace db	
	{
		namespace odbc	
		{
			class odbc_result;
			class statement 
			: public interface_statement
			, public std::enable_shared_from_this<statement>
			{
				friend class odbc_result;
				
			public:
				typedef std::shared_ptr<statement>	shared_type;

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
				virtual interface_statement& push(object, std::size_t /*= SQL_NTS*/) override;
				virtual interface_statement& push(const cyng::table::key_type&, cyng::table::meta_table_ptr) override;
				virtual bool bind(int, object&, std::size_t column_size) override;
				virtual bool bind(const cyng::table::key_type&, cyng::table::meta_table_ptr) override;
				virtual bool bind(const cyng::table::key_type&, cyng::table::data_type const&, cyng::table::meta_table_ptr) override;

				/**
				 * cleanup a complete table
				 */
				virtual void clear() override;
				
				virtual result_ptr get_result() override;
				
			public:
				//	NULL data argument
				static SQLLEN	null_data_;
				static SQLLEN	datetime_data_;
				
			private:
				bool is_valid() const;
				//bool bind_null(int index);
				//bool bind_value(int index, std::string const* value);
				//bool bind_value(int, std::int8_t);
				//bool bind_value(int, std::uint8_t);
				//bool bind_value(int, std::int16_t);
				//bool bind_value(int, std::uint16_t);
				//bool bind_value(int, std::int32_t);
				//bool bind_value(int, std::uint32_t);
				//bool bind_value(int, std::int64_t);
				//bool bind_value(int, std::uint64_t);
				//bool bind_value(int, bool);
				//bool bind_value(int, float);
				//bool bind_value(int, double);
				//bool bind_value(int, cyng::buffer_t const*);
				//bool bind_value(int, cyng::version);
				//bool bind_value(int, cyng::revision);
				//bool bind_value(int, cyng::chrono::days);
				//bool bind_value(int, std::chrono::nanoseconds);
				//bool bind_value(int, std::chrono::microseconds);
				//bool bind_value(int, std::chrono::milliseconds);
				//bool bind_value(int, std::chrono::seconds);
				//bool bind_value(int, std::chrono::minutes);
				//bool bind_value(int, std::chrono::hours);
				//bool bind_value(int, std::chrono::system_clock::time_point);
				//bool bind_value(int, cyng::attr_t const&);
				//bool bind_value(int, cyng::param_t const&);
				//
				////	m2m data types
				//bool bind_value(int, cyng::m2m::obis const&);

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

#endif	//	CYNG_DB_ODBC_STATEMENT_H

