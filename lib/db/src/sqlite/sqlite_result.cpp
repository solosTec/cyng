/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/


#include "sqlite_result.h"
#include <cyng/factory.h>
#include <cyng/parser/chrono_parser.h>
#include <cyng/intrinsics/traits/tag.hpp>
//#include <cyy/parser/chrono_parser.h>
//#include <cyy/parser/boost_parser.h>
//#include <cyy/parser/object_parser.h>
//#include <cyy/parser/mac_parser.h>
//#include <cyy/object_type.h>

#include <utility>
#include <boost/numeric/conversion/converter.hpp>
#include <boost/uuid/string_generator.hpp>

//
//	support for m2m data types
//
//#include <noddy/m2m/intrinsics/type_traits.hpp>
//#include <noddy/m2m/intrinsics/factory/obis_factory.h>
//#include <noddy/m2m/intrinsics/factory/ctrl_address_factory.h>

namespace cyng	
{
	namespace db	
	{
		namespace sqlite	
		{
			namespace detail 
			{
				template < typename T >
				object get_value(sqlite3_stmt*, int index)
				{
					return make_object();
				}
				
				template <>
				object get_value<bool>(sqlite3_stmt* stmt, int index)
				{
					const bool result = (::sqlite3_column_int(stmt, index) == 1);
					return make_object(result);
				}

				template <>
				object get_value<std::int8_t>(sqlite3_stmt* stmt, int index)
				{
					const std::int8_t result = boost::numeric::converter<std::int8_t, int>::convert(::sqlite3_column_int(stmt, index));
					return make_object(result);
				}
				
				template <>
				object get_value<std::uint8_t>(sqlite3_stmt* stmt, int index)
				{
					const std::uint8_t result = boost::numeric::converter<std::uint8_t, int>::convert(::sqlite3_column_int(stmt, index));
					return make_object(result);
				}
				
				template <>
				object get_value<std::int16_t>(sqlite3_stmt* stmt, int index)
				{
					const std::int16_t result = boost::numeric::converter<std::int16_t, int>::convert(::sqlite3_column_int(stmt, index));
					return make_object(result);
				}
				
				template <>
				object get_value<std::uint16_t>(sqlite3_stmt* stmt, int index)
				{
					const std::uint16_t result = boost::numeric::converter<std::uint16_t, int>::convert(::sqlite3_column_int(stmt, index));
					return make_object(result);
				}

				template <>
				object get_value<std::int32_t>(sqlite3_stmt* stmt, int index)
				{
					const std::int32_t result = ::sqlite3_column_int(stmt, index);
					return make_object(result);
				}
				
				template <>
				object get_value<std::uint32_t>(sqlite3_stmt* stmt, int index)
				{
					const std::uint32_t result = boost::numeric::converter<std::uint32_t, int>::convert(::sqlite3_column_int(stmt, index));
					return make_object(result);
				}

				template <>
				object get_value<std::int64_t>(sqlite3_stmt* stmt, int index)
				{
					const std::int64_t result = ::sqlite3_column_int64(stmt, index);
					//return cyy::numeric_factory_cast<std::uint64_t>(result);
					return make_object<std::uint64_t>(result);
				}
				
				template <>
				object get_value<std::uint64_t>(sqlite3_stmt* stmt, int index)
				{
					const std::uint64_t result = boost::numeric::converter<std::uint64_t, int>::convert(::sqlite3_column_int64(stmt, index));
					return make_object(result);
				}

				template <>
				object get_value<float>(sqlite3_stmt* stmt, int index)
				{
					const float result = boost::numeric::converter<float, double>::convert(::sqlite3_column_double(stmt, index));
					return make_object(result);
				}
				
				template <>
				object get_value<double>(sqlite3_stmt* stmt, int index)
				{
					const double result = ::sqlite3_column_double(stmt, index);
					return make_object(result);
				}
				
				template <>
				object get_value<long double>(sqlite3_stmt* stmt, int index)
				{
					const long double result = ::sqlite3_column_double(stmt, index);
					return make_object(result);
				}
				
				template <>
				object get_value<std::string>(sqlite3_stmt* stmt, int index)
				{
					const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
					if (ptr != NULL)
					{
						int size = ::sqlite3_column_bytes(stmt, index);
						const std::string result((const char*)ptr, size);
						BOOST_ASSERT(result.size() == static_cast<std::size_t>(size));
						return make_object(result);
					}
					return make_object();
				}
				
				template <>
				object get_value<std::chrono::system_clock::time_point>(sqlite3_stmt* stmt, int index)
				{
					const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
					if (ptr != NULL)
					{
						int size = ::sqlite3_column_bytes(stmt, index);
						const std::string result((const char*)ptr, size);
						BOOST_ASSERT(result.size() == static_cast<std::size_t>(size));
						//	format is "2014-11-28 11:06:44"
						//	parse time stamp
						return parse_rfc3339_obj(result);
					}
					return make_object();
				}
				
				template <>
				object get_value<std::chrono::nanoseconds>(sqlite3_stmt* stmt, int index)
				{
					const std::chrono::nanoseconds::rep result = ::sqlite3_column_int64(stmt, index);
					return make_nanoseconds(result);
				}
				
				template <>
				object get_value<std::chrono::microseconds>(sqlite3_stmt* stmt, int index)
				{
					const std::chrono::microseconds::rep result = ::sqlite3_column_int64(stmt, index);
					return make_microseconds(result);
				}
				
				template <>
				object get_value<std::chrono::milliseconds>(sqlite3_stmt* stmt, int index)
				{
					const std::chrono::milliseconds::rep result = ::sqlite3_column_int64(stmt, index);
					return make_milliseconds(result);
				}
				
				template <>
				object get_value<std::chrono::seconds>(sqlite3_stmt* stmt, int index)
				{
					const std::chrono::seconds::rep result = ::sqlite3_column_int64(stmt, index);
					return make_seconds(result);
				}
				
				template <>
				object get_value<std::chrono::minutes>(sqlite3_stmt* stmt, int index)
				{
					const std::chrono::minutes::rep result = ::sqlite3_column_int64(stmt, index);
					return make_minutes(result);
				}
				
				template <>
				object get_value<std::chrono::hours>(sqlite3_stmt* stmt, int index)
				{
					const std::chrono::hours::rep result = ::sqlite3_column_int64(stmt, index);
					return make_hours(result);
				}
				
				template <>
				object get_value<cyng::chrono::days>(sqlite3_stmt* stmt, int index)
				{
					const cyng::chrono::days::rep result = ::sqlite3_column_int64(stmt, index);
					return make_days(result);
				}
				
				template <>
				object get_value<version>(sqlite3_stmt* stmt, int index)
				{
					const auto result = boost::numeric::converter<std::uint32_t, int>::convert(::sqlite3_column_int(stmt, index));
					return make_object<version>(result);
				}
				
				template <>
				object get_value<revision>(sqlite3_stmt* stmt, int index)
				{
					const auto result = boost::numeric::converter<std::uint64_t, int>::convert(::sqlite3_column_int64(stmt, index));
					return make_object<version>(result);
				}
				
				template <>
				object get_value<buffer_t>(sqlite3_stmt* stmt, int index)
				{
					/* The pointer returned by sqlite3_column_blob() points to memory
					** that is owned by the statement handle (pStmt). It is only good
					** until the next call to an sqlite3_XXX() function (e.g. the
					** sqlite3_finalize() below) that involves the statement handle.
					** So we need to make a copy of the blob into memory obtained from
					** resize() to return to the caller.
					*/
					buffer_t result;
					int size = ::sqlite3_column_bytes(stmt, index);
					if (size > 0)	{
						result.resize(size);
						const char * p = static_cast<const char*>(::sqlite3_column_blob(stmt, index));
						result.assign(p, p + size);
						return make_object(result);
					}
					return make_object();
				}
				
				//	std::size_t
				//template <>
				//object get_value<cyy::index>(sqlite3_stmt* stmt, int index)
				//{
				//	const auto result = boost::numeric::converter<std::size_t, int>::convert(::sqlite3_column_int64(stmt, index));
				//	return cyy::index_factory(result);
				//}
				//
				////	std::ptrdiff_t
				//template <>
				//object get_value<cyy::diff>(sqlite3_stmt* stmt, int index)
				//{
				//	const std::ptrdiff_t result = ::sqlite3_column_int64(stmt, index);
				//	return cyy::diff_factory(result);
				//}
				
				//	cyy::mac48
				template <>
				object get_value<mac48>(sqlite3_stmt* stmt, int index)
				{
					const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
					if (ptr != NULL)
					{
						int size = ::sqlite3_column_bytes(stmt, index);
						BOOST_ASSERT_MSG(size == 17, "invalid MAC48 format");
						std::string input((const char*)ptr, size);

						//	call mac parser
						//const auto r = cyy::io::parse_mac48(input);
						//return (r.second)
						//	? make_object(r.first)
						//	: make_object()
						//	;
					}
					return make_object();
				}

				//	cyy::mac64
				template <>
				object get_value<mac64>(sqlite3_stmt* stmt, int index)
				{
					const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
					if (ptr != NULL)
					{
						int size = ::sqlite3_column_bytes(stmt, index);
						BOOST_ASSERT_MSG(size == 19, "invalid MAC64 format");
						std::string input((const char*)ptr, size);

						//	call mac parser
						//const auto r = cyy::io::parse_mac64(input);
						//return (r.second)
						//	? make_object(r.first)
						//	: make_object()
						//	;
					}
					return make_object();
				}

				//	cyy::attr_map_t
				template <>
				object get_value<attr_map_t>(sqlite3_stmt* stmt, int index)
				{
					const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
					if (ptr != NULL)
					{
						int size = ::sqlite3_column_bytes(stmt, index);
						std::string result;
						result.reserve(size + 3);
						result = "%[" + std::string((const char*)ptr, size) + "]";

						//	call object parser
						//auto r = cyy::io::parse_obj(result);
						//return (r.second)
						//	? r.first
						//	: make_object()
						//	;
					}
					return make_object();
				}

				//	param_map_t
				template <>
				object get_value<param_map_t>(sqlite3_stmt* stmt, int index)
				{
					const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
					if (ptr != NULL)
					{
						int size = ::sqlite3_column_bytes(stmt, index);
						std::string result;
						result.reserve(size + 3);
						result = "%(" + std::string((const char*)ptr, size) + ")";
						
						//	call object parser
						//auto r = cyy::io::parse_obj(result);
						//return (r.second)
						//	? r.first
						//	: make_object()
						//	;
					}
					return make_object();
				}

				template <>
				object get_value<boost::uuids::uuid>(sqlite3_stmt* stmt, int index)
				{
					const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
					if (ptr != NULL)
					{
						int size = ::sqlite3_column_bytes(stmt, index);
						const std::string str((const char*)ptr, size);
						BOOST_ASSERT(str.size() == static_cast<std::size_t>(size));
						BOOST_ASSERT(str.size() == 36);
						
						boost::uuids::string_generator gen;
						const boost::uuids::uuid result = gen(str);
						return make_object(result);
					}
					return make_object();
				}
				
				template <>
				object get_value<boost::filesystem::path>(sqlite3_stmt* stmt, int index)
				{
					const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
					if (ptr != NULL)
					{
						int size = ::sqlite3_column_bytes(stmt, index);
						const std::string str((const char*)ptr, size);
						BOOST_ASSERT(str.size() == static_cast<std::size_t>(size));
						BOOST_ASSERT(str.size() < 1024);
						
						return make_object<boost::filesystem::path>(str);
					}
					return make_object();
				}
				
				template <>
				object get_value<boost::asio::ip::address>(sqlite3_stmt* stmt, int index)
				{
					const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
					if (ptr != NULL)
					{
						int size = ::sqlite3_column_bytes(stmt, index);
						const std::string str((const char*)ptr, size);
						BOOST_ASSERT(str.size() == static_cast<std::size_t>(size));
						BOOST_ASSERT(str.size() < 40);
						//return cyy::io::parse_ip_address(str).first;
					}
					return make_object();
				}
				
				template <>
				object get_value<boost::asio::ip::tcp::endpoint>(sqlite3_stmt* stmt, int index)
				{
					const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
					if (ptr != NULL)
					{
						int size = ::sqlite3_column_bytes(stmt, index);
						const std::string str((const char*)ptr, size);
						BOOST_ASSERT(str.size() == static_cast<std::size_t>(size));
						BOOST_ASSERT(str.size() < 48);
						//return cyy::io::parse_tcp_endpoint(str).first;
					}
					return make_object();
				}
				
				//template <>
				//object get_value<cyng::m2m::obis>(sqlite3_stmt* stmt, int index)
				//{
				//	const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
				//	if (ptr != NULL)
				//	{
				//		int size = ::sqlite3_column_bytes(stmt, index);
				//		const std::string str((const char*)ptr, size);
				//		BOOST_ASSERT(str.size() == static_cast<std::size_t>(size));
				//		BOOST_ASSERT(str.size() > 10);
				//		return cyy::obis_factory(str);
				//	}
				//	return make_object();
				//}

				//template <>
				//object get_value<cyng::m2m::ctrl_address>(sqlite3_stmt* stmt, int index)
				//{
				//	const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
				//	if (ptr != NULL)
				//	{
				//		int size = ::sqlite3_column_bytes(stmt, index);
				//		const std::string str((const char*)ptr, size);
				//		BOOST_ASSERT(str.size() == static_cast<std::size_t>(size));
				//		//BOOST_ASSERT(str.size() == 17);
				//		return cyy::ctrl_address_factory(str);
				//	}
				//	return make_object();
				//}
				
			}
			
			template < type_code C >
			object get_value_by_code(sqlite3_stmt* stmt, int index)
			{
				return detail::get_value<typename cyng::traits::reverse_type<C>::type>(stmt, index - 1);
			}
			
			sqlite_result::sqlite_result(statement::shared_type stmt)
			: statement_(stmt)
			{
				BOOST_ASSERT_MSG(statement_, "no valid statement");
				BOOST_ASSERT_MSG(statement_->state_ == statement::RUNNING, "RUNNING state expected");
			}
				
			result_ptr sqlite_result::factory(statement::shared_type stmt)
			{
				auto ptr = std::make_shared<sqlite_result>(stmt);
				return std::static_pointer_cast<interface_result>(ptr->shared_from_this());
			}
			
			object sqlite_result::get(int index, std::size_t code, std::size_t /*size*/ )
			{
				BOOST_ASSERT_MSG(index != 0, "index out of range");
				try	
				{
				
					/**
					* Column index in SQLite3 starts with 0
					*/
					switch (code)
					{
						case TC_BOOL:		return get_value_by_code<TC_BOOL>(*statement_, index);
						case TC_INT8:		return get_value_by_code<TC_INT8>(*statement_, index);
						case TC_UINT8:		return get_value_by_code<TC_UINT8>(*statement_, index);
						case TC_INT16:		return get_value_by_code<TC_INT16>(*statement_, index);
						case TC_UINT16:	return get_value_by_code<TC_UINT16>(*statement_, index);
						case TC_INT32:		return get_value_by_code<TC_INT32>(*statement_, index);
						case TC_UINT32:	return get_value_by_code<TC_UINT32>(*statement_, index);
						case TC_INT64:		return get_value_by_code<TC_INT64>(*statement_, index);
						case TC_UINT64:	return get_value_by_code<TC_UINT64>(*statement_, index);

						case TC_FLOAT:	return get_value_by_code<TC_FLOAT>(*statement_, index);
						case TC_DOUBLE:	return get_value_by_code<TC_DOUBLE>(*statement_, index);
						case TC_FLOAT80:	return get_value_by_code<TC_FLOAT80>(*statement_, index);
	
						//	standard library
						case TC_STRING:	return get_value_by_code<TC_STRING>(*statement_, index);
	// 
						case TC_TIME_POINT:	return get_value_by_code<TC_TIME_POINT>(*statement_, index);
						case TC_NANO_SECOND:	return get_value_by_code<TC_NANO_SECOND>(*statement_, index);
						case TC_MICRO_SECOND:	return get_value_by_code<TC_MICRO_SECOND>(*statement_, index);
						case TC_MILLI_SECOND:	return get_value_by_code<TC_MILLI_SECOND>(*statement_, index);
						case TC_SECOND:	return get_value_by_code<TC_SECOND>(*statement_, index);
						case TC_MINUTE:	return get_value_by_code<TC_MINUTE>(*statement_, index);
						case TC_HOUR:		return get_value_by_code<TC_HOUR>(*statement_, index);
						//case cyy::types::CYY_DAYS:		return get_value_by_code<cyy::types::CYY_DAYS>(*statement_, index);
						
	// 			//	data types defined in cyy library
						case TC_VERSION:	return get_value_by_code<TC_VERSION>(*statement_, index);
						case TC_REVISION:	return get_value_by_code<TC_REVISION>(*statement_, index);
	// 			CYY_OP,			//!<	VM operation
						case TC_BUFFER:	return get_value_by_code<TC_BUFFER>(*statement_, index);
						//case cyy::types::CYY_INDEX:		return get_value_by_code<cyy::types::CYY_INDEX>(*statement_, index);
						//case cyy::types::CYY_DIFF:		return get_value_by_code<cyy::types::CYY_DIFF>(*statement_, index);
	// 			TC_COLOR_8,		//!<	color with 8 bits per channel
	// 			TC_COLOR_16,	//!<	color with 16 bits per channel
						case TC_MAC48:			return get_value_by_code<TC_MAC48>(*statement_, index);
						case TC_MAC64:			return get_value_by_code<TC_MAC64>(*statement_, index);
							// 			CYY_MD5,	//!<	md5 digest
	// 			CYY_SHA1,	//!<	sha1 digest
	// 			CYY_SHA512,	//!<	sha512 digest
	// 			
						//	std::map< std::size_t, object >
						case TC_ATTR_MAP:	return get_value_by_code<TC_ATTR_MAP>(*statement_, index);
						//	std::map< std::string, object >
						case TC_PARAM_MAP:	return get_value_by_code<TC_PARAM_MAP>(*statement_, index);
	// 			CYY_ATTRIBUTE,	//!<	std::pair<std::size_t, object>
	// 			CYY_PARAMETER,	//!<	std::pair<std::string, object>
	// 			CYY_TUPLE,		//!<	std::list<object>
	// 			CYY_VECTOR,		//!<	std::vector<object>
	// 			CYY_SET,		//!<	std::set<object>
	// 			CYY_LOCKABLE,		//!<	std::pair<T,mutex>
	// 			CYY_TABLE,		//!<	defined in CYY store library
	// 			
	// 			//	datatypes from boost library
						case TC_UUID:	return get_value_by_code<TC_UUID>(*statement_, index);
						case TC_FS_PATH:	return get_value_by_code<TC_FS_PATH>(*statement_, index);
	// 			CYY_BOOST_ERROR,	//!<	boost::system::error_code
						case TC_IP_ADDRESS:		return get_value_by_code<TC_IP_ADDRESS>(*statement_, index);
						case TC_IP_TCP_ENDPOINT: 	return get_value_by_code<TC_IP_TCP_ENDPOINT>(*statement_, index);
	// 			CYY_BOOST_TCP_SOCKET,	//!<	boost::asio::ip::tcp::socket
	// 			CYY_BOOST_TRIBOOL,		//!<	boost::logic::tribool

							//
							//	m2m data types
							//
						//case cyng::types::OBIS_CODE:		return get_value_by_code<cyng::types::OBIS_CODE>(*statement_, index);
						//case cyng::types::CTRL_ADDRESS:	return get_value_by_code<cyng::types::CTRL_ADDRESS>(*statement_, index);
						//case cyng::types::PHYSICAL_UNIT:	//	a physical unit
							break;

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

			std::size_t sqlite_result::column_count()
			{
				return ::sqlite3_column_count(*statement_);
			}

		}
	}
}

