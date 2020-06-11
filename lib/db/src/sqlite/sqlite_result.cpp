/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/


#include "sqlite_result.h"
#include <cyng/factory.h>
#include <cyng/intrinsics/traits/tag.hpp>
#include <cyng/parser/chrono_parser.h>
#include <cyng/parser/mac_parser.h>
#include <cyng/parser/buffer_parser.h>

#include <utility>
#include <boost/numeric/conversion/converter.hpp>
#include <boost/uuid/string_generator.hpp>


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
					auto const n = sqlite3_column_int(stmt, index);
					try {
						return make_object(boost::numeric::converter<std::int8_t, int>::convert(n));
					}
					catch (boost::bad_numeric_cast const ex) {
						std::cerr
							<< "get_value<std::int8_t>("
							<< n
							<< ") "
							<< ex.what()
							<< std::endl
							;
					}
					return make_object<std::int8_t>(0);
				}
				
				template <>
				object get_value<std::uint8_t>(sqlite3_stmt* stmt, int index)
				{
					auto const n = sqlite3_column_int(stmt, index);
					try {
						return make_object(boost::numeric::converter<std::uint8_t, int>::convert(n));
					}
					catch (boost::bad_numeric_cast const ex) {
						std::cerr
							<< "get_value<std::uint8_t>("
							<< n
							<< ", index "
							<< index
							<< ") "
							<< ex.what()
							<< std::endl
							;
					}
					return make_object<std::uint8_t>(0);
				}
				
				template <>
				object get_value<std::int16_t>(sqlite3_stmt* stmt, int index)
				{
					auto const n = sqlite3_column_int(stmt, index);
					try {
						return make_object(boost::numeric::converter<std::int16_t, int>::convert(n));
					}
					catch (boost::bad_numeric_cast const ex) {
						std::cerr
							<< "get_value<std::int16_t>("
							<< n
							<< ", index "
							<< index
							<< ") "
							<< ex.what()
							<< std::endl
							;
					}
					return make_object<std::int16_t>(0);
				}
				
				template <>
				object get_value<std::uint16_t>(sqlite3_stmt* stmt, int index)
				{
					auto const n = sqlite3_column_int(stmt, index);
					try {
						return make_object(boost::numeric::converter<std::uint16_t, int>::convert(n));
					}
					catch (boost::bad_numeric_cast const ex) {
						std::cerr
							<< "get_value<std::uint16_t>("
							<< n
							<< ", index "
							<< index
							<< ") "
							<< ex.what()
							<< std::endl
							;
					}
					return make_object<std::uint16_t>(0);
				}

				template <>
				object get_value<std::int32_t>(sqlite3_stmt* stmt, int index)
				{
					auto const n = sqlite3_column_int(stmt, index);
					try {
						return make_object(boost::numeric::converter<std::int32_t, int>::convert(n));
					}
					catch (boost::bad_numeric_cast const ex) {
						std::cerr
							<< "get_value<std::int32_t>("
							<< n
							<< ", index "
							<< index
							<< ") "
							<< ex.what()
							<< std::endl
							;
					}
					return make_object<std::int32_t>(0);
				}
				
				template <>
				object get_value<std::uint32_t>(sqlite3_stmt* stmt, int index)
				{
					auto const n = sqlite3_column_int(stmt, index);
					try {
						return make_object(boost::numeric::converter<std::uint32_t, int>::convert(n));
					}
					catch (boost::bad_numeric_cast const ex) {
						std::cerr
							<< "get_value<std::uint32_t>("
							<< n
							<< ", index "
							<< index
							<< ") "
							<< ex.what()
							<< std::endl
							;
					}
					return make_object<std::uint32_t>(0);
				}

				template <>
				object get_value<std::int64_t>(sqlite3_stmt* stmt, int index)
				{
					return make_object<std::uint64_t>(sqlite3_column_int64(stmt, index));
				}
				
				template <>
				object get_value<std::uint64_t>(sqlite3_stmt* stmt, int index)
				{
					auto const n = sqlite3_column_int64(stmt, index);
					try {
						return make_object(boost::numeric_cast<std::uint64_t>(n));
					}
					catch (boost::bad_numeric_cast const ex) {
						std::cerr
							<< "get_value<std::uint64_t>("
							<< n
							<< ", index "
							<< index
							<< ") "
							<< ex.what()
							<< std::endl
							;
					}
					return make_object<std::uint64_t>(0);
				}

				template <>
				object get_value<float>(sqlite3_stmt* stmt, int index)
				{
					auto const f = sqlite3_column_double(stmt, index);
					try {
						return make_object(boost::numeric::converter<float, double>::convert(f));
					}
					catch (boost::bad_numeric_cast const ex) {
						std::cerr
							<< "get_value<float>("
							<< f
							<< ", index "
							<< index
							<< ") "
							<< ex.what()
							<< std::endl
							;
					}
					return make_object<float>(0.f);
				}
				
				template <>
				object get_value<double>(sqlite3_stmt* stmt, int index)
				{
					return make_object<double>(sqlite3_column_double(stmt, index));
				}
				
				template <>
				object get_value<long double>(sqlite3_stmt* stmt, int index)
				{
					return make_object<long double>(sqlite3_column_double(stmt, index));
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
						std::pair<std::chrono::system_clock::time_point, bool > r = parse_db_timestamp(result);
						return (r.second)
							? make_object(r.first)
							: make_now()
							;
						//return parse_rfc3339_obj(result);
					}
					return make_object();
				}
				
				template <>
				object get_value<std::chrono::nanoseconds>(sqlite3_stmt* stmt, int index)
				{
					const auto result = ::sqlite3_column_int64(stmt, index);
					return make_nanoseconds(result);
				}
				
				template <>
				object get_value<std::chrono::microseconds>(sqlite3_stmt* stmt, int index)
				{
					const auto result = ::sqlite3_column_int64(stmt, index);
					return make_microseconds(result);
				}
				
				template <>
				object get_value<std::chrono::milliseconds>(sqlite3_stmt* stmt, int index)
				{
					const auto result = ::sqlite3_column_int64(stmt, index);
					return make_milliseconds(result);
				}
				
				template <>
				object get_value<std::chrono::seconds>(sqlite3_stmt* stmt, int index)
				{
					const auto result = ::sqlite3_column_int64(stmt, index);
					return make_seconds(result);
				}
				
				template <>
				object get_value<std::chrono::minutes>(sqlite3_stmt* stmt, int index)
				{
					const auto result = ::sqlite3_column_int64(stmt, index);
					return make_minutes(result);
				}
				
				template <>
				object get_value<std::chrono::hours>(sqlite3_stmt* stmt, int index)
				{
					const auto result = ::sqlite3_column_int64(stmt, index);
					return make_hours(result);
				}
				
				template <>
				object get_value<cyng::chrono::days>(sqlite3_stmt* stmt, int index)
				{
					const auto result = ::sqlite3_column_int64(stmt, index);
					return make_days(result);
				}
				
				template <>
				object get_value<version>(sqlite3_stmt* stmt, int index)
				{
					auto const n = sqlite3_column_int(stmt, index);
					try {
						return make_object<version>(boost::numeric::converter<std::uint32_t, int>::convert(n));
					}
					catch (boost::bad_numeric_cast const ex) {
						std::cerr
							<< "get_value<version>("
							<< n
							<< ", index "
							<< index
							<< ") "
							<< ex.what()
							<< std::endl
							;
					}
					return make_object<version>(0u);

				}
				
				template <>
				object get_value<revision>(sqlite3_stmt* stmt, int index)
				{
					const auto result = boost::numeric_cast<std::uint64_t>(::sqlite3_column_int64(stmt, index));
					return make_object<revision>(result);
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
								
				//	cyng::mac48
				template <>
				object get_value<mac48>(sqlite3_stmt* stmt, int index)
				{
					const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
					if (ptr != NULL)
					{
						int size = ::sqlite3_column_bytes(stmt, index);
						BOOST_ASSERT_MSG(size == 17, "invalid MAC48 format");
						std::string const input((const char*)ptr, size);

						//	call mac parser
						const auto r = cyng::parse_mac48(input);
						return (r.second)
							? make_object(r.first)
							: make_object()
							;
					}
					return make_object();
				}

				//	cyng::mac64
				template <>
				object get_value<mac64>(sqlite3_stmt* stmt, int index)
				{
					const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
					if (ptr != NULL)
					{
						int size = ::sqlite3_column_bytes(stmt, index);
						BOOST_ASSERT_MSG(size == 19, "invalid MAC64 format");
						std::string const input((const char*)ptr, size);

						//	call mac parser
						const auto r = cyng::parse_mac64(input);
						return (r.second)
							? make_object(r.first)
							: make_object()
							;
					}
					return make_object();
				}

				//	crypto::digest_md5
				template <>
				object get_value<crypto::digest_md5>(sqlite3_stmt* stmt, int index)
				{
					const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
					if (ptr != NULL)
					{
						int size = ::sqlite3_column_bytes(stmt, index);
						BOOST_ASSERT_MSG(size == 32, "invalid MD5 format");
						std::string const inp((const char*)ptr, size);

						std::pair<buffer_t, bool > const r = parse_hex_string(inp);
						if (r.second && r.first.size() == sizeof(crypto::digest_md5::value_type)) {

							crypto::digest_md5::value_type val;
							std::copy(r.first.begin(), r.first.end(), val.begin());
							return make_object(crypto::digest_md5(std::move(val)));
						}
					}
					return make_object();
				}

				//	crypto::digest_sha1
				template <>
				object get_value<crypto::digest_sha1>(sqlite3_stmt* stmt, int index)
				{
					const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
					if (ptr != NULL)
					{
						int size = ::sqlite3_column_bytes(stmt, index);
						BOOST_ASSERT_MSG(size == 40, "invalid SHA1 format");
						std::string const inp((const char*)ptr, size);

						std::pair<buffer_t, bool > const r = parse_hex_string(inp);
						if (r.second && r.first.size() == sizeof(crypto::digest_sha1::value_type)) {

							crypto::digest_sha1::value_type val;
							std::copy(r.first.begin(), r.first.end(), val.begin());
							return make_object(crypto::digest_sha1(std::move(val)));
						}
					}
					return make_object();
				}

				//	crypto::digest_sha256
				template <>
				object get_value<crypto::digest_sha256>(sqlite3_stmt* stmt, int index)
				{
					const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
					if (ptr != NULL)
					{
						int size = ::sqlite3_column_bytes(stmt, index);
						BOOST_ASSERT_MSG(size == 64, "invalid SHA256 format");
						std::string const inp((const char*)ptr, size);

						std::pair<buffer_t, bool > const r = parse_hex_string(inp);
						if (r.second && r.first.size() == sizeof(crypto::digest_sha256::value_type)) {

							crypto::digest_sha256::value_type val;
							std::copy(r.first.begin(), r.first.end(), val.begin());
							return make_object(crypto::digest_sha256(std::move(val)));
						}
					}
					return make_object();
				}

				//	crypto::digest_sha512
				template <>
				object get_value<crypto::digest_sha512>(sqlite3_stmt* stmt, int index)
				{
					const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
					if (ptr != NULL)
					{
						int size = ::sqlite3_column_bytes(stmt, index);
						BOOST_ASSERT_MSG(size == 128, "invalid SHA512 format");
						std::string const inp((const char*)ptr, size);

						std::pair<buffer_t, bool > const r = parse_hex_string(inp);
						if (r.second && r.first.size() == sizeof(crypto::digest_sha512::value_type)) {

							crypto::digest_sha512::value_type val;
							std::copy(r.first.begin(), r.first.end(), val.begin());
							return make_object(crypto::digest_sha512(std::move(val)));
						}
					}
					return make_object();
				}

				//	crypto::aes_128_key
				template <>
				object get_value<crypto::aes_128_key>(sqlite3_stmt* stmt, int index)
				{
					const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
					if (ptr != NULL)
					{
						int size = ::sqlite3_column_bytes(stmt, index);
						BOOST_ASSERT_MSG(size == 32, "invalid AESkey128 format");
						std::string const inp((const char*)ptr, size);

						std::pair<buffer_t, bool > const r = parse_hex_string(inp);
						if (r.second && r.first.size() == sizeof(crypto::aes_128_key::key_type)) {

							crypto::aes_128_key::key_type key;
							std::copy(r.first.begin(), r.first.end(), key.begin());
							return make_object(crypto::aes_128_key(std::move(key)));
						}
					}
					return make_object();
				}

				//	crypto::aes_192_key
				template <>
				object get_value<crypto::aes_192_key>(sqlite3_stmt* stmt, int index)
				{
					const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
					if (ptr != NULL)
					{
						int size = ::sqlite3_column_bytes(stmt, index);
						BOOST_ASSERT_MSG(size == 48, "invalid AESkey192 format");
						std::string const inp((const char*)ptr, size);

						std::pair<buffer_t, bool > const r = parse_hex_string(inp);
						if (r.second && r.first.size() == sizeof(crypto::aes_192_key::key_type)) {

							crypto::aes_192_key::key_type key;
							std::copy(r.first.begin(), r.first.end(), key.begin());
							return make_object(crypto::aes_192_key(std::move(key)));
						}
					}
					return make_object();
				}

				//	crypto::aes_256_key
				template <>
				object get_value<crypto::aes_256_key>(sqlite3_stmt* stmt, int index)
				{
					const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
					if (ptr != NULL)
					{
						int size = ::sqlite3_column_bytes(stmt, index);
						BOOST_ASSERT_MSG(size == 64, "invalid AESkey256 format");
						std::string const inp((const char*)ptr, size);

						std::pair<buffer_t, bool > const r = parse_hex_string(inp);
						if (r.second && r.first.size() == sizeof(crypto::aes_256_key::key_type)) {

							crypto::aes_256_key::key_type key;
							std::copy(r.first.begin(), r.first.end(), key.begin());
							return make_object(crypto::aes_256_key(std::move(key)));
						}
					}
					return make_object();
				}

				//	cyng::attr_map_t
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
						//auto r = cyng::io::parse_obj(result);
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
						//auto r = cyng::io::parse_obj(result);
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
				object get_value<filesystem::path>(sqlite3_stmt* stmt, int index)
				{
					const unsigned char* ptr = ::sqlite3_column_text(stmt, index);
					if (ptr != NULL)
					{
						int size = ::sqlite3_column_bytes(stmt, index);
						const std::string str((const char*)ptr, size);
						BOOST_ASSERT(str.size() == static_cast<std::size_t>(size));
						BOOST_ASSERT(str.size() < 1024);
						
						return make_object<filesystem::path>(str);
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
						//return cyng::io::parse_ip_address(str).first;
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
						//return cyng::io::parse_tcp_endpoint(str).first;
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
				//		return cyng::obis_factory(str);
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
				//		return cyng::ctrl_address_factory(str);
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
						//case cyng::types::CYNG_DAYS:		return get_value_by_code<cyng::types::CYNG_DAYS>(*statement_, index);
						
	// 			//	data types defined in cyng library
						case TC_VERSION:	return get_value_by_code<TC_VERSION>(*statement_, index);
						case TC_REVISION:	return get_value_by_code<TC_REVISION>(*statement_, index);
	// 			CYNG_OP,			//!<	VM operation
						case TC_BUFFER:	return get_value_by_code<TC_BUFFER>(*statement_, index);
						//case cyng::types::CYNG_INDEX:		return get_value_by_code<cyng::types::CYNG_INDEX>(*statement_, index);
						//case cyng::types::CYNG_DIFF:		return get_value_by_code<cyng::types::CYNG_DIFF>(*statement_, index);
	// 			TC_COLOR_8,		//!<	color with 8 bits per channel
	// 			TC_COLOR_16,	//!<	color with 16 bits per channel
						case TC_MAC48:			return get_value_by_code<TC_MAC48>(*statement_, index);
						case TC_MAC64:			return get_value_by_code<TC_MAC64>(*statement_, index);

						case TC_DIGEST_MD5:		return get_value_by_code<TC_DIGEST_MD5>(*statement_, index);
						case TC_DIGEST_SHA1:	return get_value_by_code<TC_DIGEST_SHA1>(*statement_, index);
						case TC_DIGEST_SHA256:	return get_value_by_code<TC_DIGEST_SHA256>(*statement_, index);
						case TC_DIGEST_SHA512:	return get_value_by_code<TC_DIGEST_SHA512>(*statement_, index);

						case TC_AES128:			return get_value_by_code<TC_AES128>(*statement_, index);
						case TC_AES192:			return get_value_by_code<TC_AES192>(*statement_, index);
						case TC_AES256:			return get_value_by_code<TC_AES256>(*statement_, index);

	// 			
						//	std::map< std::size_t, object >
						case TC_ATTR_MAP:	return get_value_by_code<TC_ATTR_MAP>(*statement_, index);
						//	std::map< std::string, object >
						case TC_PARAM_MAP:	return get_value_by_code<TC_PARAM_MAP>(*statement_, index);
	// 			CYNG_ATTRIBUTE,	//!<	std::pair<std::size_t, object>
	// 			CYNG_PARAMETER,	//!<	std::pair<std::string, object>
	// 			CYNG_TUPLE,		//!<	std::list<object>
	// 			CYNG_VECTOR,		//!<	std::vector<object>
	// 			CYNG_SET,		//!<	std::set<object>
	// 			CYNG_LOCKABLE,		//!<	std::pair<T,mutex>
	// 			CYNG_TABLE,		//!<	defined in CYNG store library
	// 			
	// 			//	datatypes from boost library
						case TC_UUID:	return get_value_by_code<TC_UUID>(*statement_, index);
						case TC_FS_PATH:	return get_value_by_code<TC_FS_PATH>(*statement_, index);
	// 			CYNG_BOOST_ERROR,	//!<	boost::system::error_code
						case TC_IP_ADDRESS:		return get_value_by_code<TC_IP_ADDRESS>(*statement_, index);
						case TC_IP_TCP_ENDPOINT: 	return get_value_by_code<TC_IP_TCP_ENDPOINT>(*statement_, index);
	// 			CYNG_BOOST_TCP_SOCKET,	//!<	boost::asio::ip::tcp::socket
	// 			CYNG_BOOST_TRIBOOL,		//!<	boost::logic::tribool

							break;

						default:
							break;
					}
				}
				catch (std::exception const& ex)
				{
					std::cerr 
						<< __FILE__
						<< ':'
						<< __LINE__
						<< ' '
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

