/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#include "odbc_statement.h"
#include "odbc_diag.hpp"
#include "odbc_result.h"
#include <cyng/io/serializer.h>
#include <cyng/value_cast.hpp>
#include <cyng/object_cast.hpp>
#include <cyng/factory/chrono_factory.h>
#include <boost/uuid/uuid_io.hpp>

namespace cyng	
{
	namespace db	
	{
		namespace odbc	
		{
			namespace detail
			{

				bool bind_parameter(SQLHSTMT stmt
					, SQLSMALLINT index
					, SQLSMALLINT value_type
					, SQLSMALLINT param_type
					, SQLPOINTER value
					, SQLLEN length
					, SQLULEN column_size)	
				{
					// Bind on internal buffer	
					SQLSMALLINT scale = 0;
					BOOST_ASSERT_MSG(param_type != SQL_TIMESTAMP, "wrong bind method" );
					const SQLRETURN rc = ::SQLBindParameter(stmt
						, index				//	column index
						, SQL_PARAM_INPUT	//	InputOutputType
						, value_type		//	ValueType
						, param_type		//	ParameterType
						, column_size		//	ColumnSize
						, scale				//	DecimalDigits
						, value				//	ParameterValuePtr
						, length			//	BufferLength
						, NULL);			//	result size
					return SQL_SUCCEEDED(rc);
				}
				
				/**
				*	If you want to insert NULL data, the last parameter of SQLBindParameter
				*	should contain SQL_NULL_DATA
				*/
				bool bind_null(SQLHSTMT stmt
					, SQLSMALLINT idx
					, SQLSMALLINT param_type
					, SQLULEN column_size) 
				{
					const SQLRETURN rc = ::SQLBindParameter(stmt
						, idx
						, SQL_PARAM_INPUT
						, SQL_C_DEFAULT //	value type
						, param_type	//	SQL_NUMERIC, SQL_VARCHAR, ...
						, column_size	//	column size
						, 0				//	decimal digits
						, NULL			//	value
						, 0				//	length
						, &statement::null_data_);  //	SQL_NULL_DATA
					return SQL_SUCCEEDED(rc);
				}
				
				bool bind_datetime(SQLHSTMT stmt
					, SQLSMALLINT idx
					, SQLPOINTER value) 
				{
					const SQLRETURN rc = ::SQLBindParameter(stmt
						, idx
						, SQL_PARAM_INPUT
						, SQL_C_TIMESTAMP //	value type
						, SQL_TYPE_TIMESTAMP
						//	, 22		//	column size - MySQL Server
						, 27		//	column size	- MS SQL Server
						, 7			//	decimal digits
						, value			//	value
						, sizeof(SQL_TIMESTAMP_STRUCT)	//	length
						, &statement::datetime_data_);  //	sizeof(SQL_TIMESTAMP_STRUCT);
					return SQL_SUCCEEDED(rc);
				}

				template < typename T >
				bool bind_value(SQLHSTMT stmt, SQLSMALLINT index, T const* ptr, std::size_t column_size)
				{
#ifdef _DEBUG
					//std::cerr << cyy::demangle<T>() << " NOT SEPCIALIZED" << std::endl;
#endif
					return false;
				}

				template <>
				bool bind_value<bool>(SQLHSTMT stmt, SQLSMALLINT index, bool const* ptr, std::size_t column_size)
				{
					return (ptr != nullptr)
					? bind_parameter(stmt, index, SQL_C_BIT, SQL_BIT, (SQLPOINTER)ptr, 1, column_size)
					: bind_null(stmt, index, SQL_BIT, 1);
				}

				template <>
				bool bind_value<std::int8_t>(SQLHSTMT stmt, SQLSMALLINT index, std::int8_t const* ptr, std::size_t column_size)
				{
					return (ptr != nullptr)
					? bind_parameter(stmt, index, SQL_C_TINYINT, SQL_TINYINT, (SQLPOINTER)ptr, sizeof(std::int8_t), column_size)
					: bind_null(stmt, index, SQL_TINYINT, 1);
				}

				template <>
				bool bind_value<std::uint8_t>(SQLHSTMT stmt, SQLSMALLINT index, std::uint8_t const* ptr, std::size_t column_size)
				{
					return (ptr != nullptr)
					? bind_parameter(stmt, index, SQL_C_UTINYINT, SQL_TINYINT, (SQLPOINTER)ptr, sizeof(std::uint8_t), column_size)
					: bind_null(stmt, index, SQL_TINYINT, 1);
				}

				template <>
				bool bind_value<std::int16_t>(SQLHSTMT stmt, SQLSMALLINT index, std::int16_t const* ptr, std::size_t column_size)
				{
					return (ptr != nullptr)
					? bind_parameter(stmt, index, SQL_C_SSHORT, SQL_SMALLINT, (SQLPOINTER)ptr, sizeof(std::int16_t), column_size)
					: bind_null(stmt, index, SQL_SMALLINT, 6);
				}

				template <>
				bool bind_value<std::uint16_t>(SQLHSTMT stmt, SQLSMALLINT index, std::uint16_t const* ptr, std::size_t column_size)
				{
					return (ptr != nullptr)
					? bind_parameter(stmt, index, SQL_C_USHORT, SQL_SMALLINT, (SQLPOINTER)ptr, sizeof(std::int16_t), column_size)
					: bind_null(stmt, index, SQL_SMALLINT, 5);
				}

				template <>
				bool bind_value<std::int32_t>(SQLHSTMT stmt, SQLSMALLINT index, std::int32_t const* ptr, std::size_t column_size)
				{
					return (ptr != nullptr)
					? bind_parameter(stmt, index, SQL_C_SLONG, SQL_INTEGER, (SQLPOINTER)ptr, sizeof(std::int32_t), column_size)
					: bind_null(stmt, index, SQL_INTEGER, 11);
				}

				template <>
				bool bind_value<std::uint32_t>(SQLHSTMT stmt, SQLSMALLINT index, std::uint32_t const* ptr, std::size_t column_size)
				{
					return (ptr != nullptr)
					? bind_parameter(stmt, index, SQL_C_ULONG, SQL_INTEGER, (SQLPOINTER)ptr, sizeof(std::uint32_t), column_size)
					: bind_null(stmt, index, SQL_INTEGER, 10);
				}

				template <>
				bool bind_value<std::int64_t>(SQLHSTMT stmt, SQLSMALLINT index, std::int64_t const* ptr, std::size_t column_size)
				{
					return (ptr != nullptr)
					? bind_parameter(stmt, index, SQL_C_SBIGINT, SQL_BIGINT, (SQLPOINTER)ptr, sizeof(std::int64_t), column_size)
					: bind_null(stmt, index, SQL_BIGINT, 21);
				}

				template <>
				bool bind_value<std::uint64_t>(SQLHSTMT stmt, SQLSMALLINT index, std::uint64_t const* ptr, std::size_t column_size)
				{
					return (ptr != nullptr)
					? bind_parameter(stmt, index, SQL_C_UBIGINT, SQL_BIGINT, (SQLPOINTER)ptr, sizeof(std::uint64_t), column_size)
					: bind_null(stmt, index, SQL_BIGINT, 20);
				}

				template <>
				bool bind_value<float>(SQLHSTMT stmt, SQLSMALLINT index, float const* ptr, std::size_t column_size)
				{
					return (ptr != nullptr)
					? bind_parameter(stmt, index, SQL_C_FLOAT, SQL_REAL, (SQLPOINTER)ptr, sizeof(float), column_size)
					: bind_null(stmt, index, SQL_REAL, 23);
				}

				template <>
				bool bind_value<double>(SQLHSTMT stmt, SQLSMALLINT index, double const* ptr, std::size_t column_size)
				{
					return (ptr != nullptr)
					? bind_parameter(stmt, index, SQL_C_DOUBLE, SQL_DOUBLE, (SQLPOINTER)ptr, sizeof(double), column_size)
					: bind_null(stmt, index, SQL_DOUBLE, 53);
				}

				template <>
				bool bind_value<std::string>(SQLHSTMT stmt, SQLSMALLINT index, std::string const* ptr, std::size_t column_size)
				{
					return (ptr != nullptr)
					? bind_parameter(stmt, index, SQL_C_CHAR, SQL_CHAR, (SQLPOINTER)ptr->c_str(), ptr->size(), column_size)
					: bind_null(stmt, index, SQL_CHAR, 128);
				}

				template <>
				bool bind_value<buffer_t>(SQLHSTMT stmt, SQLSMALLINT index, buffer_t const* ptr, std::size_t column_size)
				{
					return (ptr != nullptr)
						? bind_parameter(stmt, index, SQL_C_BINARY, SQL_BINARY, (SQLPOINTER)ptr->data(), ptr->size(), column_size)
						: bind_null(stmt, index, SQL_CHAR, 128);
				}

				template <>
				bool bind_value<SQL_TIMESTAMP_STRUCT>(SQLHSTMT stmt, SQLSMALLINT index, SQL_TIMESTAMP_STRUCT const* ptr, std::size_t column_size)
				{
					return (ptr != nullptr)
					? bind_datetime(stmt, index, (SQLPOINTER)ptr)
					: bind_null(stmt, index, SQL_CHAR, column_size);
				}

				template <>
				bool bind_value<std::chrono::nanoseconds>(SQLHSTMT stmt, SQLSMALLINT index, std::chrono::nanoseconds const* ptr, std::size_t column_size)
				{
					return (ptr != nullptr)
						? bind_parameter(stmt, index, SQL_C_SBIGINT, SQL_BIGINT, (SQLPOINTER)ptr, sizeof(std::int64_t), column_size)
						: bind_null(stmt, index, SQL_BIGINT, column_size);
				}

				template <>
				bool bind_value<std::chrono::microseconds>(SQLHSTMT stmt, SQLSMALLINT index, std::chrono::microseconds const* ptr, std::size_t column_size)
				{
					return (ptr != nullptr)
						? bind_parameter(stmt, index, SQL_C_SBIGINT, SQL_BIGINT, (SQLPOINTER)ptr, sizeof(std::int64_t), column_size)
						: bind_null(stmt, index, SQL_BIGINT, column_size);
				}

				template <>
				bool bind_value<std::chrono::milliseconds>(SQLHSTMT stmt, SQLSMALLINT index, std::chrono::milliseconds const* ptr, std::size_t column_size)
				{
					return (ptr != nullptr)
						? bind_parameter(stmt, index, SQL_C_SBIGINT, SQL_BIGINT, (SQLPOINTER)ptr, sizeof(std::int64_t), column_size)
						: bind_null(stmt, index, SQL_BIGINT, column_size);
				}

				template <>
				bool bind_value<std::chrono::seconds>(SQLHSTMT stmt, SQLSMALLINT index, std::chrono::seconds const* ptr, std::size_t column_size)
				{
					return (ptr != nullptr)
						? bind_parameter(stmt, index, SQL_C_SBIGINT, SQL_BIGINT, (SQLPOINTER)ptr, sizeof(std::int64_t), column_size)
						: bind_null(stmt, index, SQL_BIGINT, column_size);
				}

				template <>
				bool bind_value<std::chrono::minutes>(SQLHSTMT stmt, SQLSMALLINT index, std::chrono::minutes const* ptr, std::size_t column_size)
				{
					return (ptr != nullptr)
						? bind_parameter(stmt, index, SQL_C_SBIGINT, SQL_BIGINT, (SQLPOINTER)ptr, sizeof(std::int64_t), column_size)
						: bind_null(stmt, index, SQL_BIGINT, column_size);
				}

				template <>
				bool bind_value<std::chrono::hours>(SQLHSTMT stmt, SQLSMALLINT index, std::chrono::hours const* ptr, std::size_t column_size)
				{
					return (ptr != nullptr)
						? bind_parameter(stmt, index, SQL_C_SBIGINT, SQL_BIGINT, (SQLPOINTER)ptr, sizeof(std::int64_t), column_size)
						: bind_null(stmt, index, SQL_BIGINT, column_size);
				}

			}

			statement::statement(SQLHDBC h)
			: stmt_(h)
			, state_(stmt_.is_valid() ? STATE_INITIALIZED : STATE_ERROR)
			, bind_counter_(1)
			, static_data_()
			{}
				
			statement::~statement()
			{
//				stop();
			}

			bool statement::is_state(state s) const 
			{
				return s == state_;
			}

			void statement::tidy() 
			{

				if (is_state(STATE_TO_CLOSE)) 
				{

					if (close()) 
					{

						//	reset state
						state_ = (is_state(STATE_PREPARED))
							? STATE_PREPARED
							: STATE_INITIALIZED
							;
					}
				}
			}

			void statement::stop() 
			{
				if (is_state(STATE_POSITIONED)) 
				{
					state_ = STATE_TO_CLOSE;
				}
				tidy();
			}

			bool statement::execute()
			{
				//	execute
				const SQLRETURN rc = ::SQLExecute(stmt_);
				if (SQL_SUCCEEDED(rc)) 
				{
					state_ = STATE_EXECUTED;
					return true;
				}
				state_ = STATE_ERROR;
				statement_diagnostics dia;
				dia.run(stmt_);
				return false;
			}
			
			bool statement::execute(std::string const& sql)
			{
				return (prepare(sql).second) 
					? execute()
					: false 
				;
			}

			std::pair<int, bool> statement::prepare(std::string const& sql)
			{
				//	A statement can prepared only once
				BOOST_ASSERT_MSG(state_ == STATE_INITIALIZED, "STATE_INITIALIZED expected");
				const SQLRETURN rc = ::SQLPrepare(stmt_, (SQLCHAR*)sql.c_str(), static_cast<SQLINTEGER>(sql.length()));
				if (SQL_SUCCEEDED(rc)) 
				{
					state_ = STATE_PREPARED;
//					prepared_ = true;
					SQLSMALLINT params{ 0 };
					::SQLNumParams(stmt_, &params);
					return std::make_pair(params, true);
				}
				state_ = STATE_ERROR;
				statement_diagnostics dia;
				dia.run(stmt_);
//				return false;
				return std::make_pair(0, false);
			}
			
			bool statement::close()
			{
				//	A statement can prepared only once
				BOOST_ASSERT_MSG(state_ == STATE_TO_CLOSE || state_ == STATE_ERROR, "STATE_TO_CLOSE expected");

				//	close cursor
				const SQLRETURN rc = ::SQLFreeStmt(stmt_, SQL_CLOSE);
				if (SQL_SUCCEEDED(rc)) 
				{
					return true;
				}
				state_ = STATE_ERROR;
				statement_diagnostics dia;
				dia.run(stmt_);
				return false;
			}

			//	data binding
			interface_statement& statement::push(object obj, std::size_t column_size)
			{
				//
				//	Error: swapping of objects doesn't work
				//
				if (bind(bind_counter_, obj, column_size))
				{
					bind_counter_++;
				}
				return *this;
			}

			interface_statement& statement::push(const cyng::table::key_type& key, cyng::table::meta_table_ptr tbl)
			{
				if (bind(key, tbl))
				{
					bind_counter_ += static_cast<int>(key.size());
				}
				return *this;
			}
			
			result_ptr statement::get_result()
			{
				switch (state_)
				{
					case STATE_PREPARED:
						return (execute() && state_ == STATE_EXECUTED)
							? odbc_result::factory(shared_from_this())
							: result_ptr()
							;
					case STATE_EXECUTED:
						//	assumes that execute() doesn't changed the state to DONE
						return (step() && state_ == STATE_POSITIONED)
							? odbc_result::factory(shared_from_this())
							: result_ptr()
							;
					case STATE_POSITIONED:
						return odbc_result::factory(shared_from_this());

					default:
						break;
				}
				return result_ptr();
			}
			
			bool statement::step() 
			{
				BOOST_ASSERT_MSG((state_ == STATE_EXECUTED) || (state_ == STATE_POSITIONED)
					, "STATE_EXECUTED or STATE_POSITIONED expected");
				// fetch data
				const SQLRETURN rc = ::SQLFetchScroll(stmt_, SQL_FETCH_NEXT, 0);
				//const SQLRETURN rc = ::SQLFetch(stmt_);
				switch (rc)
				{
				case SQL_SUCCESS:
				case SQL_SUCCESS_WITH_INFO:
					state_ = STATE_POSITIONED;
					return true;
				case SQL_NO_DATA:
					state_ = STATE_TO_CLOSE;
					return false;
				default:
					state_ = STATE_ERROR;
					break;
				}
				statement_diagnostics dia;
				dia.run(stmt_);
				return false;
			}

			template < type_code C >
			bool statement::bind_value_by_code(object obj, std::size_t column_size)
			{
				typedef typename cyng::traits::reverse_type< C >::type type;
				//static_assert(!std::is_same<type, cyy::undef>::value, "undefined type");

				BOOST_ASSERT_MSG(state_ == STATE_PREPARED, "STATE_PREPARED expected");

				static_data_.push_back(obj);
				if (!detail::bind_value< type >(stmt_.get_handle()
					, static_cast<SQLSMALLINT>(static_data_.size())
					, object_cast<type>(obj)
					, column_size))
				{
					state_ = STATE_ERROR;
					statement_diagnostics dia;
					dia.run(stmt_);

					//	remove invalid data
					static_data_.pop_back();

					return false;
				}
				return true;
			}

			
			bool statement::bind(int index, object& obj, std::size_t column_size)
			{
				switch (obj.get_class().tag())
				{	
					case TC_BOOL:		return bind_value_by_code<TC_BOOL>(obj, 1);
					case TC_INT8:		return bind_value_by_code<TC_INT8>(obj, 1);
					case TC_UINT8:		return bind_value_by_code<TC_UINT8>(obj, 1);
					case TC_INT16:		return bind_value_by_code<TC_INT16>(obj, 6);
					case TC_UINT16:		return bind_value_by_code<TC_UINT16>(obj, 5);
					case TC_INT32:		return bind_value_by_code<TC_INT32>(obj, 11);
					case TC_UINT32:		return bind_value_by_code<TC_UINT32>(obj, 10);
					case TC_INT64:		return bind_value_by_code<TC_INT64>(obj, 21);
					case TC_UINT64:		return bind_value_by_code<TC_UINT64>(obj, 20);
					case TC_FLOAT:		return bind_value_by_code<TC_FLOAT>(obj, 23);
					case TC_DOUBLE:		return bind_value_by_code<TC_DOUBLE>(obj, 53);
// 					case TC_FLOAT80:

					//	standard library
					case TC_STRING:		return bind_value_by_code<TC_STRING>(obj, column_size);
					case TC_TIME_POINT:
					{
						//	TC_TIME_POINT ==> SQL_TIMESTAMP_STRUCT
						object sql_time = make_odbc_ts(value_cast(obj, std::chrono::system_clock::time_point::min()));
						swap(obj, sql_time);
						return bind_value_by_code<TC_SQL_TIMESTAMP>(obj, 27);
					}
					case TC_NANO_SECOND:	return bind_value_by_code<TC_NANO_SECOND>(obj, 21);
					case TC_MICRO_SECOND:	return bind_value_by_code<TC_MICRO_SECOND>(obj, 21);
					case TC_MILLI_SECOND:	return bind_value_by_code<TC_MILLI_SECOND>(obj, 21);
					case TC_SECOND:			return bind_value_by_code<TC_SECOND>(obj, 21);
					case TC_MINUTE:			return bind_value_by_code<TC_MINUTE>(obj, 21);
					case TC_HOUR:			return bind_value_by_code<TC_HOUR>(obj, 21);
						
					case TC_BUFFER:			return bind_value_by_code<TC_BUFFER>(obj, column_size);

					case TC_MAC48:	
					{
						//	data type substitution 
						object str = make_object(cyng::io::to_str(obj));
						swap(obj, str);
						//	CYY_MAC48 ==> std::string
						return bind_value_by_code<TC_STRING>(obj, 17);
					}

					case TC_MAC64:
					{
						//	data type substitution 
						object str = make_object(cyng::io::to_str(obj));
						swap(obj, str);
						//	CYY_MAC64 ==> std::string
						return bind_value_by_code<TC_STRING>(obj, 19);
					}

					case TC_ATTR_MAP:
					{
						//	data type substitution 
						object str = make_object(cyng::io::to_str(obj));
						swap(obj, str);
						//	CYY_ATTR_MAP ==> std::string
						return bind_value_by_code<TC_STRING>(obj, 2048);
					}

					case TC_PARAM_MAP:
					{
						//	data type substitution 
						object str = make_object(cyng::io::to_str(obj));
						swap(obj, str);
						//	CYY_PARAM_MAP ==> std::string
						return bind_value_by_code<TC_STRING>(obj, 2048);
					}

					case TC_ATTR:
					{
						//	data type substitution 
						object str = make_object(cyng::io::to_str(obj));
						swap(obj, str);
						//	CYY_ATTRIBUTE ==> std::string
						return bind_value_by_code<TC_STRING>(obj, 1024);
					}

					case TC_PARAM:
					{
						//	data type substitution 
						object str = make_object(cyng::io::to_str(obj));
						swap(obj, str);
						//	CYY_PARAMETER ==> std::string
						return bind_value_by_code<TC_STRING>(obj, 1024);
					}

					//						
					case TC_UUID:		
					{
						//	data type substitution
						object str = make_object(cyng::io::to_str(obj));
						swap(obj, str);
						return bind_value_by_code<TC_STRING>(obj, 37);
					}
					case TC_FS_PATH:
					{
						//	boost::filesystem::path
						//	data type substitution
						object str = make_object(cyng::io::to_str(obj));
						swap(obj, str);
						return bind_value_by_code<TC_STRING>(obj, 1024);
					}

					//case TC_IP_TCP_ENDPOINT:
					//case TC_IP_UDP_ENDPOINT:
					//case TC_IP_ICMP_ENDPOINT:

					case TC_IP_ADDRESS:	
					{
						//	boost::asio::ip::address
						//	data type substitution
						object str = make_object(cyng::io::to_str(obj));
						swap(obj, str);
						return bind_value_by_code<TC_STRING>(obj, 40);
					}
						
					default:
						break;
				}
				return false;
			}
			
			bool statement::bind(const cyng::table::key_type& key, cyng::table::data_type const& data, cyng::table::meta_table_ptr meta)
			{
				//BOOST_ASSERT_MSG(data.size() == meta.body_count(), "invalid body size");
				BOOST_ASSERT_MSG(key.size() + data.size() == meta->size(), "invalid table structure");
				BOOST_ASSERT_MSG(bind_counter_ == 1, "wrong state");

				if (meta->check_record(key, data))
				{
					bind(key, meta);
					for (auto const& v : data)
					{
						push(v, meta->get_width(bind_counter_ - 1));
					}
					return true;
				}
				return false;
			}

			bool statement::bind(const cyng::table::key_type& key, cyng::table::meta_table_ptr meta)
			{
				//BOOST_ASSERT_MSG(key.size() == meta.key_count(), "invalid key size");

				if (meta->check_key(key))
				{
					for (auto const& v : key)
					{
						push(v, meta->get_width(bind_counter_ - 1));
					}
					return true;
				}
				return false;
			}

			void statement::clear()
			{
				bind_counter_ = 1;

				//	 Reset parameter bindings
				//	 all parameters are unbound 
				const SQLRETURN rc = ::SQLFreeStmt(stmt_, SQL_RESET_PARAMS);
				if (!SQL_SUCCEEDED(rc))
				{
					state_ = STATE_ERROR;
					statement_diagnostics dia;
					dia.run(stmt_);
				}
				static_data_.clear();
				state_ = STATE_INITIALIZED;
			}
			
		

			//	initialize NULL data argument
			SQLLEN	statement::null_data_ = SQL_NULL_DATA;
			SQLLEN	statement::datetime_data_ = sizeof(SQL_TIMESTAMP_STRUCT);

			statement_ptr statement_factory(SQLHDBC h)
			{
				auto ptr = std::make_shared<statement>(h);
				return std::static_pointer_cast<interface_statement>(ptr->shared_from_this());
			}

		}
	}
}

