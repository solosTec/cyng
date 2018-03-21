/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#include "odbc_result.h"
#include "odbc_diag.hpp"

#include <cyng/factory.h>
#include <cyng/parser/chrono_parser.h>
#include <cyng/object.h>
#include <cyng/value_cast.hpp>
#include <cyng/chrono.h>

#include <utility>
#include <boost/numeric/conversion/converter.hpp>
#include <boost/uuid/string_generator.hpp>


namespace cyng	
{
	namespace db	
	{
		namespace odbc	
		{
			namespace detail 
			{
				template < typename T >
				object get_value(SQLHSTMT, int index, std::size_t size)
				{
					return make_object();
				}
				
				template <>
				object get_value<bool>(SQLHSTMT stmt, int index, std::size_t size)
				{
					BOOST_ASSERT_MSG(index != 0, "index out of range");
					std::uint8_t sql_value = 0;
					SQLLEN length{ 0 };
					const SQLRETURN rc = ::SQLGetData(stmt, index, SQL_C_BIT, &sql_value, 0, &length);
					if (is_ok(rc)) 
					{
						return make_object(sql_value == 1);
					}
					statement_diagnostics dia;
					dia.run(stmt);
					return make_object();
					//const bool result = (::sqlite3_column_int(stmt, index) == 1);
					//return make_object(result);
				}

				template <>
				object get_value<std::int8_t>(SQLHSTMT stmt, int index, std::size_t)
				{
					BOOST_ASSERT_MSG(index != 0, "index out of range");
					SQLLEN length{ 0 };
					std::int8_t result{ 0 };
					const SQLRETURN rc = ::SQLGetData(stmt, index, SQL_C_STINYINT, &result, 0, &length);
					BOOST_ASSERT_MSG(length = sizeof(std::int8_t), "wrong data length (std::int8_t)");
					if (is_ok(rc))	return make_object(result);
					statement_diagnostics dia;
					dia.run(stmt);
					return make_object();
				}
				
				template <>
				object get_value<std::uint8_t>(SQLHSTMT stmt, int index, std::size_t)
				{
					BOOST_ASSERT_MSG(index != 0, "index out of range");
					SQLLEN length{ 0 };
					std::uint8_t result{ 0 };
					const SQLRETURN rc = ::SQLGetData(stmt, index, SQL_C_UTINYINT, &result, 0, &length);
					BOOST_ASSERT_MSG(length = sizeof(std::uint8_t), "wrong data length (std::uint8_t)");
					if (is_ok(rc))	return make_object(result);
					statement_diagnostics dia;
					dia.run(stmt);
					return make_object();
				}
				
				template <>
				object get_value<std::int16_t>(SQLHSTMT stmt, int index, std::size_t)
				{
					BOOST_ASSERT_MSG(index != 0, "index out of range");
					SQLLEN length{ 0 };
					std::int16_t result{ 0 };
					const SQLRETURN rc = ::SQLGetData(stmt, index, SQL_C_SSHORT, &result, 0, &length);
					BOOST_ASSERT_MSG(length = sizeof(std::int16_t), "wrong data length (std::int16_t)");
					if (is_ok(rc))	make_object(result);
					statement_diagnostics dia;
					dia.run(stmt);
					return make_object();
					//const std::int16_t result = boost::numeric::converter<std::int16_t, int>::convert(::sqlite3_column_int(stmt, index));
					//return make_object(result);
				}
				
				template <>
				object get_value<std::uint16_t>(SQLHSTMT stmt, int index, std::size_t)
				{
					BOOST_ASSERT_MSG(index != 0, "index out of range");
					SQLLEN length{ 0 };
					std::uint16_t result{ 0 };
					const SQLRETURN rc = ::SQLGetData(stmt, index, SQL_C_USHORT, &result, 0, &length);
					BOOST_ASSERT_MSG(length = sizeof(std::uint16_t), "wrong data length (std::uint16_t)");
					if (is_ok(rc))	return make_object(result);
					statement_diagnostics dia;
					dia.run(stmt);
					return make_object();
					//const std::uint16_t result = boost::numeric::converter<std::uint16_t, int>::convert(::sqlite3_column_int(stmt, index));
					//return make_object(result);
				}

				template <>
				object get_value<std::int32_t>(SQLHSTMT stmt, int index, std::size_t)
				{
					BOOST_ASSERT_MSG(index != 0, "index out of range");
					SQLLEN length{ 0 };
					std::int32_t result = { 0 };
					const SQLRETURN rc = ::SQLGetData(stmt, index, SQL_C_SLONG, &result, 0, &length);
					BOOST_ASSERT_MSG(length = sizeof(std::int32_t), "wrong data length (std::int32_t)");
					if (is_ok(rc))	return make_object(result);
					statement_diagnostics dia;
					dia.run(stmt);
					return make_object();
				}

				template <>
				object get_value<std::uint32_t>(SQLHSTMT stmt, int index, std::size_t)
				{
					BOOST_ASSERT_MSG(index != 0, "index out of range");
					SQLLEN length{ 0 };
					std::uint32_t result = { 0 };
					const SQLRETURN rc = ::SQLGetData(stmt, index, SQL_C_ULONG, &result, 0, &length);
					BOOST_ASSERT_MSG(length = sizeof(std::uint32_t), "wrong data length (std::uint32_t)");
					if (is_ok(rc))	return make_object(result);
					statement_diagnostics dia;
					dia.run(stmt);
					return make_object();
					//const std::uint32_t result = boost::numeric::converter<std::uint32_t, int>::convert(::sqlite3_column_int(stmt, index));
					//return make_object(result);
				}

				template <>
				object get_value<std::int64_t>(SQLHSTMT stmt, int index, std::size_t)
				{
					BOOST_ASSERT_MSG(index != 0, "index out of range");
					SQLLEN length{ 0 };
					std::int64_t result = { 0 };
					const SQLRETURN rc = ::SQLGetData(stmt, index, SQL_C_SBIGINT, &result, 0, &length);
					BOOST_ASSERT_MSG(length = sizeof(std::int64_t), "wrong data length (std::int64_t)");
					if (is_ok(rc))	return make_object(result);
					statement_diagnostics dia;
					dia.run(stmt);
					return make_object();
					//const std::int64_t result = ::sqlite3_column_int64(stmt, index);
					//return cyy::numeric_factory_cast<std::uint64_t>(result);
				}
				
				template <>
				object get_value<std::uint64_t>(SQLHSTMT stmt, int index, std::size_t)
				{
					BOOST_ASSERT_MSG(index != 0, "index out of range");
					SQLLEN length{ 0 };
					std::uint64_t result = { 0 };
					const SQLRETURN rc = ::SQLGetData(stmt, index, SQL_C_UBIGINT, &result, 0, &length);
					BOOST_ASSERT_MSG(length = sizeof(std::uint64_t), "wrong data length (std::uint64_t)");
					if (is_ok(rc))	return make_object(result);
					statement_diagnostics dia;
					dia.run(stmt);
					return make_object();
					//const std::uint64_t result = boost::numeric::converter<std::uint64_t, int>::convert(::sqlite3_column_int64(stmt, index));
					//return make_object(result);
				}

				template <>
				object get_value<float>(SQLHSTMT stmt, int index, std::size_t size)
				{
					BOOST_ASSERT_MSG(index != 0, "index out of range");
					SQLLEN length{ 0 };
					float result{ 0.0 };
					const SQLRETURN rc = ::SQLGetData(stmt, index, SQL_C_FLOAT, &result, 0, &length);
					BOOST_ASSERT_MSG(length = sizeof(float), "wrong data length (float)");
					if (is_ok(rc))	return make_object(result);
					statement_diagnostics dia;
					dia.run(stmt);
					return make_object();
					//const float result = boost::numeric::converter<float, double>::convert(::sqlite3_column_double(stmt, index));
					//return make_object(result);
				}
				
				template <>
				object get_value<double>(SQLHSTMT stmt, int index, std::size_t size)
				{
					//const double result = ::sqlite3_column_double(stmt, index);
					//return make_object(result);
					BOOST_ASSERT_MSG(index != 0, "index out of range");
					SQLLEN length{ 0 };
					double result{ 0.0 };
					const SQLRETURN rc = ::SQLGetData(stmt, index, SQL_C_DOUBLE, &result, 0, &length);
					BOOST_ASSERT_MSG(length = sizeof(double), "wrong data length (double)");
					if (is_ok(rc))	return make_object(result);
					statement_diagnostics dia;
					dia.run(stmt);
					return make_object();
				}
				
			//	template <>
			//	object get_value<long double>(SQLHSTMT stmt, int index, std::size_t)
			//	{
			//		const long double result = ::sqlite3_column_double(stmt, index);
			//		return make_object(result);
			//	}
			//	
				template <>
				object get_value<std::string>(SQLHSTMT stmt, int index, std::size_t size)
				{
					BOOST_ASSERT_MSG(index != 0, "index out of range");
					BOOST_ASSERT_MSG(size < 0xFFFF, "size out of range");
					SQLLEN length { 0 };
					buffer_t binvalue(size + 1);

					const SQLRETURN rc = ::SQLGetData(stmt, index, SQL_C_CHAR, binvalue.data(), size + 1, &length);
					if (is_ok(rc))
					{
						std::string result;
						if (length == -1) 
						{
							result.clear();
						}
						else 
						{
							BOOST_ASSERT_MSG(length <= static_cast<SQLLEN>(size), "wrong data length (std::string)");
							result.assign(binvalue.begin(), binvalue.begin() + length);
						}
						return make_object(result);
					}
					statement_diagnostics dia;
					dia.run(stmt);
					return make_object();
				}
				
				template <>
				object get_value<std::chrono::system_clock::time_point>(SQLHSTMT stmt, int index, std::size_t)
				{
					BOOST_ASSERT_MSG(index != 0, "index out of range");
					SQLLEN length = { 0 };
					SQL_TIMESTAMP_STRUCT sql_value;
					sql_value.year = 0;
					sql_value.month = 0;
					sql_value.day = 0;
					sql_value.hour = 0;
					sql_value.minute = 0;
					sql_value.second = 0;
					sql_value.fraction = 0;

					const SQLRETURN rc = ::SQLGetData(stmt, index, SQL_C_TIMESTAMP, &sql_value, sizeof(sql_value), &length);
					if (is_ok(rc)) {
						BOOST_ASSERT_MSG(length = sizeof(SQL_TIMESTAMP_STRUCT), "wrong data length (time_point)");
						BOOST_ASSERT_MSG(sql_value.year > 1900, "year is out of range");
						BOOST_ASSERT_MSG(sql_value.month > 0 && sql_value.month < 13, "month is out of range");
						BOOST_ASSERT_MSG(sql_value.day >= 0 && sql_value.day < 32, "day is out of range");
						BOOST_ASSERT_MSG(sql_value.hour >= 0 && sql_value.hour < 24, "hour is out of range");
						BOOST_ASSERT_MSG(sql_value.minute >= 0 && sql_value.minute < 60, "min is out of range");
						BOOST_ASSERT_MSG(sql_value.second >= 0 && sql_value.second < 60, "second is out of range");

						std::tm t;
						t.tm_sec = sql_value.second;
						t.tm_min = sql_value.minute;
						t.tm_hour = sql_value.hour;
						t.tm_mday = sql_value.day;
						t.tm_mon = sql_value.month - 1;
						t.tm_year = sql_value.year - 1900;
						t.tm_isdst = 0;	//	ignore DST

						const auto tp = chrono::init_tp(sql_value.year
							, static_cast<std::uint8_t>(sql_value.month)
							, static_cast<std::uint8_t>(sql_value.day)
							, static_cast<std::uint8_t>(sql_value.hour)
							, static_cast<std::uint8_t>(sql_value.minute)
							, sql_value.second);
						return make_object(tp);
					}
					statement_diagnostics dia;
					dia.run(stmt);
					return make_object();
				}
			//	
			//	template <>
			//	object get_value<std::chrono::nanoseconds>(SQLHSTMT stmt, int index, std::size_t)
			//	{
			//		const std::chrono::nanoseconds::rep result = ::sqlite3_column_int64(stmt, index);
			//		return cyy::nanoseconds_factory(result);
			//	}
			//	
			//	template <>
			//	object get_value<std::chrono::microseconds>(SQLHSTMT stmt, int index, std::size_t)
			//	{
			//		const std::chrono::microseconds::rep result = ::sqlite3_column_int64(stmt, index);
			//		return cyy::microseconds_factory(result);
			//	}
			//	
			//	template <>
			//	object get_value<std::chrono::milliseconds>(SQLHSTMT stmt, int index, std::size_t)
			//	{
			//		const std::chrono::milliseconds::rep result = ::sqlite3_column_int64(stmt, index);
			//		return cyy::milliseconds_factory(result);
			//	}
			//	
			//	template <>
			//	object get_value<std::chrono::seconds>(SQLHSTMT stmt, int index, std::size_t)
			//	{
			//		const std::chrono::seconds::rep result = ::sqlite3_column_int64(stmt, index);
			//		return cyy::seconds_factory(result);
			//	}
			//	
			//	template <>
			//	object get_value<std::chrono::minutes>(SQLHSTMT stmt, int index, std::size_t)
			//	{
			//		const std::chrono::minutes::rep result = ::sqlite3_column_int64(stmt, index);
			//		return cyy::minutes_factory(result);
			//	}
			//	
			//	template <>
			//	object get_value<std::chrono::hours>(SQLHSTMT stmt, int index, std::size_t)
			//	{
			//		const std::chrono::hours::rep result = ::sqlite3_column_int64(stmt, index);
			//		return cyy::hours_factory(result);
			//	}
			//	
			//	template <>
			//	object get_value<cyy::chrono::days>(SQLHSTMT stmt, int index, std::size_t)
			//	{
			//		const cyy::chrono::days::rep result = ::sqlite3_column_int64(stmt, index);
			//		return cyy::days_factory(result);
			//	}
			//	
				template <>
				object get_value<version>(SQLHSTMT stmt, int index, std::size_t size)
				{
					BOOST_ASSERT_MSG(index != 0, "index out of range");
					//	version is stored as std::uint32_t
					const std::uint32_t result = value_cast<std::uint32_t>(get_value<std::uint32_t>(stmt, index, size), 0);
					return make_object<version>(result);
				}
				
				template <>
				object get_value<revision>(SQLHSTMT stmt, int index, std::size_t size)
				{
					BOOST_ASSERT_MSG(index != 0, "index out of range");
					//	version is stored as std::uint64_t
					const std::uint64_t result = value_cast<std::uint64_t>(get_value<std::uint64_t>(stmt, index, size), 1);
					return make_object<revision>(result);
				}
				
				template <>
				object get_value<buffer_t>(SQLHSTMT stmt, int index, std::size_t size)
				{
					BOOST_ASSERT_MSG(index != 0, "index out of range");
					//BOOST_ASSERT_MSG(size < 0xFFFF, "size out of range");
					SQLLEN length = { 0 };
					buffer_t result(size + 1);

					//
					//	ToDo: retrieve data in distinct parts
					//
					SQLINTEGER  bytes{ 0 };
					while (SQLRETURN rc = ::SQLGetData(stmt, index, SQL_C_BINARY, result.data(), size + 1, &length) != SQL_NO_DATA)
					{
						bytes += static_cast<SQLINTEGER>(length);
					}
					return make_object(result);
					//if (is_ok(rc))
					//{
					//	std::string result;
					//	if (length == -1)
					//	{
					//		result.clear();
					//	}
					//	else
					//	{
					//		BOOST_ASSERT_MSG(length <= size, "wrong data length (std::string)");
					//	}
					//	return make_object(result);
					//}
					//statement_diagnostics dia;
					//dia.run(stmt);
					//return make_object();

					//cyy::buffer_t result;
					//int size = ::sqlite3_column_bytes(stmt, index);
					//if (size > 0)	{
					//	result.resize(size);
					//	const char * p = static_cast<const char*>(::sqlite3_column_blob(stmt, index));
					//	result.assign(p, p + size);
					//	return make_object(result);
					//}
					//return make_object();
				}
			//	
			//	//	std::size_t
			//	template <>
			//	object get_value<cyy::index>(SQLHSTMT stmt, int index, std::size_t)
			//	{
			//		const auto result = boost::numeric::converter<std::size_t, int>::convert(::sqlite3_column_int64(stmt, index));
			//		return cyy::index_factory(result);
			//	}
			//	
			//	//	std::ptrdiff_t
			//	template <>
			//	object get_value<cyy::diff>(SQLHSTMT stmt, int index)
			//	{
			//		const std::ptrdiff_t result = ::sqlite3_column_int64(stmt, index);
			//		return cyy::diff_factory(result);
			//	}
				
				//	cyy::mac48
				template <>
				object get_value<mac48>(SQLHSTMT stmt, int index, std::size_t size)
				{
					BOOST_ASSERT_MSG(index != 0, "index out of range");
					std::string str = value_cast<std::string>(get_value<std::string>(stmt, index, 18), "");
					if (str.size() > 17)
					{
						//	call mac parser
						//const auto r = cyy::io::parse_mac48(str);
						//return (r.second)
						//	?make_object(r.first)
						//	:make_object()
						//	;
					}
					return make_object();
				}

				//	cyy::mac64
				template <>
				object get_value<mac64>(SQLHSTMT stmt, int index, std::size_t size)
				{
					BOOST_ASSERT_MSG(index != 0, "index out of range");
					std::string str = value_cast<std::string>(get_value<std::string>(stmt, index, 19), "");
					if (str.size() > 18)
					{
						//	call mac parser
						//const auto r = cyy::io::parse_mac48(str);
						//return (r.second)
						//	?make_object(r.first)
						//	:make_object()
						//	;
					}
					return make_object();
				}

			//	//	cyy::attr_map_t
			//	template <>
			//	object get_value<cyy::attr_map_t>(SQLHSTMT stmt, int index)
			//	{
			//		const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
			//		if (ptr != NULL)
			//		{
			//			int size = ::sqlite3_column_bytes(stmt, index);
			//			std::string result;
			//			result.reserve(size + 3);
			//			result = "%[" + std::string((const char*)ptr, size) + "]";

			//			//	call object parser
			//			auto r = cyy::io::parse_obj(result);
			//			return (r.second)
			//				? r.first
			//				:make_object()
			//				;
			//		}
			//		return make_object();
			//	}

			//	//	cyy::param_map_t
			//	template <>
			//	object get_value<cyy::param_map_t>(SQLHSTMT stmt, int index)
			//	{
			//		const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
			//		if (ptr != NULL)
			//		{
			//			int size = ::sqlite3_column_bytes(stmt, index);
			//			std::string result;
			//			result.reserve(size + 3);
			//			result = "%(" + std::string((const char*)ptr, size) + ")";
			//			
			//			//	call object parser
			//			auto r = cyy::io::parse_obj(result);
			//			return (r.second)
			//				? r.first
			//				:make_object()
			//				;
			//		}
			//		return make_object();
			//	}

				template <>
				object get_value<boost::uuids::uuid>(SQLHSTMT stmt, int index, std::size_t size)
				{
					BOOST_ASSERT_MSG(index != 0, "index out of range");
					std::string str = value_cast<std::string>(get_value<std::string>(stmt, index, 36), "");
					if (str.size() == 36) 
					{
						static boost::uuids::string_generator gen;
						return make_object(gen(str));
					}
					return make_object();
				}
				
			//	template <>
			//	object get_value<boost::filesystem::path>(SQLHSTMT stmt, int index, std::size_t size)
			//	{
			//		const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
			//		if (ptr != NULL)
			//		{
			//			int size = ::sqlite3_column_bytes(stmt, index);
			//			const std::string str((const char*)ptr, size);
			//			BOOST_ASSERT(str.size() == static_cast<std::size_t>(size));
			//			BOOST_ASSERT(str.size() < 1024);
			//			
			//			return cyy::path_factory(str);
			//		}
			//		return make_object();
			//	}
			//	
			//	template <>
			//	object get_value<boost::asio::ip::address>(SQLHSTMT stmt, int index, std::size_t size)
			//	{
			//		const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
			//		if (ptr != NULL)
			//		{
			//			int size = ::sqlite3_column_bytes(stmt, index);
			//			const std::string str((const char*)ptr, size);
			//			BOOST_ASSERT(str.size() == static_cast<std::size_t>(size));
			//			BOOST_ASSERT(str.size() < 40);
			//			return cyy::io::parse_ip_address(str).first;
			//		}
			//		return make_object();
			//	}
			//	
			//	template <>
			//	object get_value<boost::asio::ip::tcp::endpoint>(SQLHSTMT stmt, int index, std::size_t size)
			//	{
			//		const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
			//		if (ptr != NULL)
			//		{
			//			int size = ::sqlite3_column_bytes(stmt, index);
			//			const std::string str((const char*)ptr, size);
			//			BOOST_ASSERT(str.size() == static_cast<std::size_t>(size));
			//			BOOST_ASSERT(str.size() < 48);
			//			return cyy::io::parse_tcp_endpoint(str).first;
			//		}
			//		return make_object();
			//	}
			//	

				
			}	//	detail
			
			template < type_code C >
			object get_value_by_code(SQLHSTMT stmt, int index, std::size_t size)
			{
				return detail::get_value<typename cyng::traits::reverse_type<C>::type>(stmt, index, size);
			}
			
			odbc_result::odbc_result(statement::shared_type stmt)
				: statement_(stmt)
			{
				BOOST_ASSERT_MSG(statement_, "no valid statement");
				//BOOST_ASSERT_MSG(statement_->state_ == statement::RUNNING, "RUNNING state expected");
				statement_->step();
			}
				
			//	static factory method
			result_ptr odbc_result::factory(statement::shared_type stmt)
			{
				auto ptr = std::make_shared<odbc_result>(stmt);
				return (stmt->state_ != statement::STATE_TO_CLOSE)
					? std::static_pointer_cast<interface_result>(ptr->shared_from_this())
					: result_ptr()
					;
			}
			
			object odbc_result::get(int index, std::size_t code, std::size_t size)
			{
				BOOST_ASSERT_MSG(index != 0, "index out of range");
				try	
				{
				
					/**
					 * Column index in ODBC starts with 1
					 */
					switch (code)
					{
						case TC_BOOL:		return get_value_by_code<TC_BOOL>(statement_->stmt_, index, size);
						case TC_INT8:		return get_value_by_code<TC_INT8>(statement_->stmt_, index, size);
						case TC_UINT8:		return get_value_by_code<TC_UINT8>(statement_->stmt_, index, size);
						case TC_INT16:		return get_value_by_code<TC_INT16>(statement_->stmt_, index, size);
						case TC_UINT16:		return get_value_by_code<TC_UINT16>(statement_->stmt_, index, size);
						case TC_INT32:		return get_value_by_code<TC_INT32>(statement_->stmt_, index, size);
						case TC_UINT32:		return get_value_by_code<TC_UINT32>(statement_->stmt_, index, size);
						case TC_INT64:		return get_value_by_code<TC_INT64>(statement_->stmt_, index, size);
						case TC_UINT64:		return get_value_by_code<TC_UINT64>(statement_->stmt_, index, size);

						case TC_FLOAT:		return get_value_by_code<TC_FLOAT>(statement_->stmt_, index, size);
						case TC_DOUBLE:		return get_value_by_code<TC_DOUBLE>(statement_->stmt_, index, size);
						case TC_FLOAT80:	return get_value_by_code<TC_FLOAT80>(statement_->stmt_, index, size);
	//
	//					//	standard library
						case TC_STRING:	return get_value_by_code<TC_STRING>(statement_->stmt_, index, size);
						case TC_TIME_POINT:	return get_value_by_code<TC_TIME_POINT>(statement_->stmt_, index, size);
	//					
	 				//	data types defined in cyng library
						case TC_VERSION:	return get_value_by_code<TC_VERSION>(statement_->stmt_, index, size);
						case TC_REVISION:	return get_value_by_code<TC_REVISION>(statement_->stmt_, index, size);
						case TC_BUFFER:		return get_value_by_code<TC_BUFFER>(statement_->stmt_, index, size);
	//// 			TC_COLOR_8,		//!<	color with 8 bits per channel
	//// 			TC_COLOR_16,	//!<	color with 16 bits per channel
	//// 			
	//// 			//	datatypes from boost library
						case TC_UUID:	return get_value_by_code<TC_UUID>(statement_->stmt_, index, size);
						//case TC_FS_PATH:	return get_value_by_code<TC_FS_PATH>(statement_->stmt_, index, size);
							// 			CYY_BOOST_ERROR,	//!<	boost::system::error_code
						//case TC_IP_ADDRESS:		return get_value_by_code<TC_IP_ADDRESS>(statement_->stmt_, index, size);
						//case TC_IP_TCP_ENDPOINT: 	return get_value_by_code<TC_IP_TCP_ENDPOINT>(statement_->stmt_, index, size);

						default:
							break;
					}
				}
				catch (boost::numeric::negative_overflow const& ex)
				{
					std::cerr 
					<< ex.what()
					<< std::endl
					;
				}
				catch (boost::numeric::positive_overflow const& ex)
				{
					std::cerr 
					<< ex.what()
					<< std::endl
					;
				}
				return make_object();
			}

			std::size_t odbc_result::column_count()
			{
				SQLLEN   row_count = 0;
				::SQLRowCount(statement_->stmt_, &row_count);
				return row_count;
			}

		}	//	odbc
	}	//	db
}

