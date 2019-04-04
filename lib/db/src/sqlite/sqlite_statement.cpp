/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/


#include "sqlite_statement.h"
#include "sqlite_result.h"
#include <cyng/io/serializer.h>
#include <cyng/intrinsics/traits/tag.hpp>
#include <cyng/object_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace cyng	
{
	namespace db	
	{
		namespace sqlite	
		{
			namespace detail
			{

				template < typename T >
				bool bind_value(sqlite3_stmt* stmt, int index, T const* ptr)
				{
					return (ptr != nullptr)
						? is_ok(::sqlite3_bind_int(stmt, index, *ptr))
						: is_ok(::sqlite3_bind_null(stmt, index))
						;
				}

				template <>
				bool bind_value< null >(sqlite3_stmt* stmt, int index, null const* ptr)
				{
					return is_ok(::sqlite3_bind_null(stmt, index));
				}

				template <>
				bool bind_value< std::int64_t >(sqlite3_stmt* stmt, int index, std::int64_t const* ptr)
				{
					return (ptr != nullptr)
						? is_ok(::sqlite3_bind_int64(stmt, index, *ptr))
						: is_ok(::sqlite3_bind_null(stmt, index))
						;
				}

				template <>
				bool bind_value< std::uint64_t >(sqlite3_stmt* stmt, int index, std::uint64_t const* ptr)
				{
					//
					//	There is a limitation in SQLite3 that only signed 64 bit integers
					//	are supported. But since we know the true data type we can cast 
					//	to signed integers when writing and cast back when reading.
					//
					return (ptr != nullptr)
						? is_ok(::sqlite3_bind_int64(stmt, index, *ptr))
						: is_ok(::sqlite3_bind_null(stmt, index))
						;
				}

				template <>
				bool bind_value< float >(sqlite3_stmt* stmt, int index, float const* ptr)
				{
					return (ptr != nullptr)
						? is_ok(::sqlite3_bind_double(stmt, index, static_cast<double>(*ptr)))
						: is_ok(::sqlite3_bind_null(stmt, index))
						;

				}

				template <>
				bool bind_value< double >(sqlite3_stmt* stmt, int index, double const* ptr)
				{
					return (ptr != nullptr)
						? is_ok(::sqlite3_bind_double(stmt, index, *ptr))
						: is_ok(::sqlite3_bind_null(stmt, index))
						;

				}

				template <>
				bool bind_value< long double >(sqlite3_stmt* stmt, int index, long double const* ptr)
				{
					return (ptr != nullptr)
						? is_ok(::sqlite3_bind_double(stmt, index, static_cast<double>(*ptr)))
						: is_ok(::sqlite3_bind_null(stmt, index))
						;

				}

				template <>
				bool bind_value< std::string >(sqlite3_stmt* stmt, int index, std::string const* ptr)
				{
					//
					//	With SQLITE_STATIC SQLite assumes that the information is in static, 
					//	unmanaged space and does not need to be freed
					//
					return (ptr != nullptr)
						? is_ok(::sqlite3_bind_text(stmt, index, ptr->c_str(), static_cast<int>(ptr->size()), SQLITE_STATIC))
						: is_ok(::sqlite3_bind_null(stmt, index))
						;
				}

				template <>
				bool bind_value< std::chrono::system_clock::time_point >(sqlite3_stmt* stmt, int index, std::chrono::system_clock::time_point const* ptr)
				{
					//	julianday() converts string representation into float
					//	yyyy-mm-dd hh:mm:ss
					if (ptr != nullptr)
					{
						//	The SQLITE_TRANSIENT value means that SQLite should make its own private copy of
						//	the content before returning.
						//	If well prepared this statement call the SQLite julianday() function and converts 
						//	the string into a float value.
						const std::string str = to_str(*ptr);
						BOOST_ASSERT_MSG(str.size() == 19, "invalid time format");
						return is_ok(::sqlite3_bind_text(stmt, index, str.c_str(), static_cast<int>(str.size()), SQLITE_TRANSIENT));
					}
					return is_ok(::sqlite3_bind_null(stmt, index));
				}

				template <>
				bool bind_value< std::chrono::nanoseconds >(sqlite3_stmt* stmt, int index, std::chrono::nanoseconds const* ptr)
				{
					if (ptr != nullptr)
					{
						const auto value = ptr->count();
						return bind_value<std::chrono::nanoseconds::rep>(stmt, index, &value);
					}
					return is_ok(::sqlite3_bind_null(stmt, index));
				}

				template <>
				bool bind_value< std::chrono::microseconds >(sqlite3_stmt* stmt, int index, std::chrono::microseconds const* ptr)
				{
					if (ptr != nullptr)
					{
						const auto value = ptr->count();
						return bind_value(stmt, index, &value);
					}
					return is_ok(::sqlite3_bind_null(stmt, index));
				}

				template <>
				bool bind_value< std::chrono::milliseconds >(sqlite3_stmt* stmt, int index, std::chrono::milliseconds const* ptr)
				{
					if (ptr != nullptr)
					{
						const auto value = ptr->count();
						return bind_value(stmt, index, &value);
					}
					return is_ok(::sqlite3_bind_null(stmt, index));
				}

				template <>
				bool bind_value< std::chrono::seconds >(sqlite3_stmt* stmt, int index, std::chrono::seconds const* ptr)
				{
					if (ptr != nullptr)
					{
						const auto value = ptr->count();
						return bind_value(stmt, index, &value);
					}
					return is_ok(::sqlite3_bind_null(stmt, index));
				}

				template <>
				bool bind_value< std::chrono::minutes >(sqlite3_stmt* stmt, int index, std::chrono::minutes const* ptr)
				{
					if (ptr != nullptr)
					{
						const auto value = ptr->count();
						return bind_value(stmt, index, &value);
					}
					return is_ok(::sqlite3_bind_null(stmt, index));
				}

				template <>
				bool bind_value< std::chrono::hours >(sqlite3_stmt* stmt, int index, std::chrono::hours const* ptr)
				{
					if (ptr != nullptr)
					{
						const auto value = ptr->count();
						return bind_value(stmt, index, &value);
					}
					return is_ok(::sqlite3_bind_null(stmt, index));
				}

				template <>
				bool bind_value< cyng::chrono::days >(sqlite3_stmt* stmt, int index, cyng::chrono::days const* ptr)
				{
					if (ptr != nullptr)
					{
						const auto value = ptr->count();
						return bind_value(stmt, index, &value);
					}
					return is_ok(::sqlite3_bind_null(stmt, index));
				}

				template <>
				bool bind_value< buffer_t >(sqlite3_stmt* stmt, int index, buffer_t const* ptr)
				{
					//
					//	SQLite assumes that the information is in static, unmanaged space and does not need to be freed
					//
					return (ptr != nullptr)
						? is_ok(::sqlite3_bind_blob(stmt, index, ptr->data(), static_cast<int>(ptr->size()), SQLITE_STATIC))
						: is_ok(::sqlite3_bind_null(stmt, index))
						;
				}

				template <>
				bool bind_value< mac48 >(sqlite3_stmt* stmt, int index, mac48 const* ptr)
				{
					if (ptr != nullptr)
					{
						//	The SQLITE_TRANSIENT value means that SQLite should make its own private copy of
						//	the content before returning.
						std::stringstream ss;
						using io::operator<<;
						ss << *ptr;
						const std::string str = ss.str();
						return is_ok(::sqlite3_bind_text(stmt, index, str.c_str(), static_cast<int>(str.size()), SQLITE_TRANSIENT));
					}
					return is_ok(::sqlite3_bind_null(stmt, index));
				}

				template <>
				bool bind_value< mac64 >(sqlite3_stmt* stmt, int index, mac64 const* ptr)
				{
					if (ptr != nullptr)
					{
						//	The SQLITE_TRANSIENT value means that SQLite should make its own private copy of
						//	the content before returning.
						std::stringstream ss;
						using io::operator<<;
						ss << *ptr;
						std::string const str = ss.str();
						return is_ok(::sqlite3_bind_text(stmt, index, str.c_str(), static_cast<int>(str.size()), SQLITE_TRANSIENT));
					}
					return is_ok(::sqlite3_bind_null(stmt, index));
				}

				template <>
				bool bind_value< crypto::aes_128_key >(sqlite3_stmt* stmt, int index, crypto::aes_128_key const* ptr)
				{
					if (ptr != nullptr)
					{
						std::stringstream ss;
						using io::operator<<;
						ss << *ptr;
						std::string const str = ss.str();
						return is_ok(::sqlite3_bind_text(stmt, index, str.c_str(), static_cast<int>(str.size()), SQLITE_TRANSIENT));
					}
					return is_ok(::sqlite3_bind_null(stmt, index));
				}

				template <>
				bool bind_value< crypto::aes_192_key >(sqlite3_stmt* stmt, int index, crypto::aes_192_key const* ptr)
				{
					if (ptr != nullptr)
					{
						std::stringstream ss;
						using io::operator<<;
						ss << *ptr;
						std::string const str = ss.str();
						return is_ok(::sqlite3_bind_text(stmt, index, str.c_str(), static_cast<int>(str.size()), SQLITE_TRANSIENT));
					}
					return is_ok(::sqlite3_bind_null(stmt, index));
				}

				template <>
				bool bind_value< crypto::aes_256_key >(sqlite3_stmt* stmt, int index, crypto::aes_256_key const* ptr)
				{
					if (ptr != nullptr)
					{
						std::stringstream ss;
						using io::operator<<;
						ss << *ptr;
						std::string const str = ss.str();
						return is_ok(::sqlite3_bind_text(stmt, index, str.c_str(), static_cast<int>(str.size()), SQLITE_TRANSIENT));
					}
					return is_ok(::sqlite3_bind_null(stmt, index));
				}

				template <>
				bool bind_value< attr_map_t >(sqlite3_stmt* stmt, int index, attr_map_t const* ptr)
				{
					if (ptr != nullptr)
					{
						//	The SQLITE_TRANSIENT value means that SQLite should make its own private copy of
						//	the content before returning.
						//const std::string str = cyng::io::to_string(*ptr, cyng::io::custom_callback());
						//return is_ok(::sqlite3_bind_text(stmt, index, str.c_str(), str.size(), SQLITE_TRANSIENT));
					}
					return is_ok(::sqlite3_bind_null(stmt, index));
				}
				
				template <>
				bool bind_value< param_map_t >(sqlite3_stmt* stmt, int index, param_map_t const* ptr)
				{
					if (ptr != nullptr)
					{
						//	The SQLITE_TRANSIENT value means that SQLite should make its own private copy of
						//	the content before returning.
						//const std::string str = cyng::io::to_string(*ptr, cyng::io::custom_callback());
						//return is_ok(::sqlite3_bind_text(stmt, index, str.c_str(), str.size(), SQLITE_TRANSIENT));
					}
					return is_ok(::sqlite3_bind_null(stmt, index));
				}

				template <>
				bool bind_value< attr_t >(sqlite3_stmt* stmt, int index, attr_t const* ptr)
				{
					if (ptr != nullptr)
					{
						//	The SQLITE_TRANSIENT value means that SQLite should make its own private copy of
						//	the content before returning.
						//const std::string str = cyng::io::to_literal(*ptr, cyng::io::custom_callback());
						//return is_ok(::sqlite3_bind_text(stmt, index, str.c_str(), str.size(), SQLITE_TRANSIENT));
					}
					return is_ok(::sqlite3_bind_null(stmt, index));
				}

				template <>
				bool bind_value< param_t >(sqlite3_stmt* stmt, int index, param_t const* ptr)
				{
					if (ptr != nullptr)
					{
						//	The SQLITE_TRANSIENT value means that SQLite should make its own private copy of
						//	the content before returning.
						//const std::string str = cyng::io::to_literal(*ptr, cyng::io::custom_callback());
						//return is_ok(::sqlite3_bind_text(stmt, index, str.c_str(), str.size(), SQLITE_TRANSIENT));
					}
					return is_ok(::sqlite3_bind_null(stmt, index));
				}

				template <>
				bool bind_value< boost::uuids::uuid >(sqlite3_stmt* stmt, int index, boost::uuids::uuid const* ptr)
				{
					if (ptr != nullptr)
					{
						//	store UUIDs as string
						const std::string str = boost::uuids::to_string(*ptr);
						//	SQLITE_TRANSIENT: make a copy of the string 
						return is_ok(::sqlite3_bind_text(stmt, index, str.c_str(), static_cast<int>(str.size()), SQLITE_TRANSIENT));
					}
					return is_ok(::sqlite3_bind_null(stmt, index));
				}

				template <>
				bool bind_value< boost::filesystem::path >(sqlite3_stmt* stmt, int index, boost::filesystem::path const* ptr)
				{
					if (ptr != nullptr)
					{
						//	store path as string
						const std::string str = ptr->generic_string();
						//	SQLITE_TRANSIENT: make a copy of the string 
						return is_ok(::sqlite3_bind_text(stmt, index, str.c_str(), static_cast<int>(str.size()), SQLITE_TRANSIENT));
					}
					return is_ok(::sqlite3_bind_null(stmt, index));
				}

				template <>
				bool bind_value< boost::asio::ip::address >(sqlite3_stmt* stmt, int index, boost::asio::ip::address const* ptr)
				{
					if (ptr != nullptr)
					{
						//	store IP address as string
						const std::string str = ptr->to_string();
						//	SQLITE_TRANSIENT: make a copy of the string 
						return is_ok(::sqlite3_bind_text(stmt, index, str.c_str(), static_cast<int>(str.size()), SQLITE_TRANSIENT));
					}
					return is_ok(::sqlite3_bind_null(stmt, index));
				}

				template <>
				bool bind_value< boost::asio::ip::tcp::endpoint >(sqlite3_stmt* stmt, int index, boost::asio::ip::tcp::endpoint const* ptr)
				{
					if (ptr != nullptr)
					{
						//	store boost::asio::ip::tcp::endpoint as string
						std::stringstream ss;
						ss << *ptr;
						const std::string str = ss.str();
						//	SQLITE_TRANSIENT: make a copy of the string 
						return is_ok(::sqlite3_bind_text(stmt, index, str.c_str(), static_cast<int>(str.size()), SQLITE_TRANSIENT));
					}
					return is_ok(::sqlite3_bind_null(stmt, index));
				}
			}

			statement::statement(connection& con)
			: connection_(con)
			, stmt_(nullptr)
			, tail_(nullptr)
			, state_(INITIAL)
			, bind_counter_(1)	//	starts with 1
			, static_data_()
			{}
				
			bool statement::execute()
			{
				BOOST_ASSERT_MSG(is_valid(), "SQLite statement invalid");
				const int rc = ::sqlite3_step(stmt_);
				switch (rc)
				{
				case SQLITE_ROW:
					//	returns any data
					state_ = RUNNING;
					return true;
				case SQLITE_DONE:
					//	no more data
					state_ = DONE;
					return true;
				default:
					break;
				}
				connection_.show_diagnostics();
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
				//BOOST_ASSERT_MSG(state_ == INITIAL, "INITIAL state expected");	
				if (state_ == RUNNING) {
					int i = 0;
				}
				if (state_ == INITIAL || state_ == RUNNING)
				{
					const int rc = ::sqlite3_prepare_v2(connection_, sql.c_str(), static_cast<int>(sql.length()), &stmt_, &tail_);
					if (!is_ok(rc))	
					{
						connection_.show_diagnostics();
						return std::make_pair(0, false);
					}
					
					//	update state
					state_ = READY;
					return std::make_pair(::sqlite3_bind_parameter_count(stmt_), true);
				}
				return std::make_pair(0, false);
			}
			
			bool statement::close()
			{
				if (state_ == RUNNING || state_ == DONE)
				{
					tail_ = nullptr;
					if (is_valid())	
					{
						const int rc = ::sqlite3_finalize(stmt_);
						stmt_ = nullptr;
						state_ = INITIAL;
						return is_ok(rc);
					}
					connection_.show_diagnostics();
				}
				return false;
			}

			//	data binding
			interface_statement& statement::push(object obj, std::size_t column_size)
			{
				BOOST_ASSERT_MSG(bind_counter_ != 0, "invalid column index");
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
					//	assumes that execute() doesn't changed the state to DONE
				case READY:
						//	invoke sqlite3_step() again, to get next row
				case RUNNING:
					if (execute()) {
						return (state_ == RUNNING)
							? sqlite_result::factory(shared_from_this())
							: result_ptr()
							;
					}
				default:
					break;
				}
				return result_ptr();
			}
			
			template < type_code C >
			bool statement::bind_value_by_code(object obj, std::size_t column_size)
			{
				using type = typename cyng::traits::reverse_type< C >::type;

				BOOST_ASSERT_MSG(is_valid(), "SQLite statement invalid");

				if (!detail::bind_value< type >(stmt_, bind_counter_, object_cast<type>(obj)))
				{
					connection_.show_diagnostics();
					return false;
				}
				return true;
			}

			bool statement::bind(int index, object& obj, std::size_t column_size)
			{
				switch (obj.get_class().tag())
				{	
					case TC_NULL:	return bind_value_by_code<TC_NULL>(obj, column_size);

					case TC_BOOL:		return bind_value_by_code<TC_BOOL>(obj, column_size);
					case TC_INT8:		return bind_value_by_code<TC_INT8>(obj, column_size);
					case TC_UINT8:		return bind_value_by_code<TC_UINT8>(obj, column_size);
					case TC_INT16:		return bind_value_by_code<TC_INT16>(obj, column_size);
					case TC_UINT16:		return bind_value_by_code<TC_UINT16>(obj, column_size);
					case TC_INT32:		return bind_value_by_code<TC_INT32>(obj, column_size);
					case TC_UINT32:		return bind_value_by_code<TC_UINT32>(obj, column_size);
					case TC_INT64:		return bind_value_by_code<TC_INT64>(obj, column_size);
					case TC_UINT64:		return bind_value_by_code<TC_UINT64>(obj, column_size);

					case TC_FLOAT:		return bind_value_by_code<TC_FLOAT>(obj, column_size);
					case TC_DOUBLE:		return bind_value_by_code<TC_DOUBLE>(obj, column_size);
 					case TC_FLOAT80:	return bind_value_by_code<TC_FLOAT80>(obj, column_size);

					//	standard library
					case TC_STRING:	
						//	hold a reference
						static_data_.push_back(obj);
						return bind_value_by_code<TC_STRING>(obj, column_size);

					case TC_TIME_POINT:	return bind_value_by_code<TC_TIME_POINT>(obj, column_size);
					case TC_NANO_SECOND:	return bind_value_by_code<TC_NANO_SECOND>(obj, column_size);
					case TC_MICRO_SECOND:	return bind_value_by_code<TC_MICRO_SECOND>(obj, column_size);
					case TC_MILLI_SECOND:	return bind_value_by_code<TC_MILLI_SECOND>(obj, column_size);
					case TC_SECOND:		return bind_value_by_code<TC_SECOND>(obj, column_size);
					case TC_MINUTE:		return bind_value_by_code<TC_MINUTE>(obj, column_size);
					case TC_HOUR:			return bind_value_by_code<TC_HOUR>(obj, column_size);
						
					case TC_BUFFER:		//!<	internal buffer type
						//	hold a reference
						static_data_.push_back(obj);
						return bind_value_by_code<TC_BUFFER>(obj, column_size);

					case TC_MAC48:			return bind_value_by_code<TC_MAC48>(obj, column_size);
					case TC_MAC64:			return bind_value_by_code<TC_MAC64>(obj, column_size);


					//
					//	some data types are not implemented yet
					//	...
					//
						
					case TC_ATTR_MAP:		return bind_value_by_code<TC_ATTR_MAP>(obj, column_size);
					case TC_PARAM_MAP:		return bind_value_by_code<TC_PARAM_MAP>(obj, column_size);
					case TC_ATTR:		return bind_value_by_code<TC_ATTR>(obj, column_size);
					case TC_PARAM:		return bind_value_by_code<TC_PARAM>(obj, column_size);

					//
					//	some data types are not implemented yet
					//	...
					//

					case TC_UUID:			return bind_value_by_code<TC_UUID>(obj, column_size);
					case TC_FS_PATH:			return bind_value_by_code<TC_FS_PATH>(obj, column_size);

					case TC_IP_TCP_ENDPOINT:	return bind_value_by_code<TC_IP_TCP_ENDPOINT>(obj, column_size);
					//case TC_IP_UDP_ENDPOINT:
					//case TC_IP_ICMP_ENDPOINT:
					case TC_IP_ADDRESS:		return bind_value_by_code<TC_IP_ADDRESS>(obj, column_size);
						
						
					default:
						break;
				}
				return false;
			}
			
			bool statement::bind(const cyng::table::key_type& key, cyng::table::data_type const& data, cyng::table::meta_table_ptr meta)
			{
				//BOOST_ASSERT_MSG(data.size() == meta->body_count(), "invalid body size");
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
				const int rc = ::sqlite3_clear_bindings(stmt_);
				if (is_ok(rc))
				{
					state_ = RUNNING;
					const int rc = ::sqlite3_reset(stmt_);
					if (!is_ok(rc))
					{
						connection_.show_diagnostics();
					}
					//state_ = INITIAL;
				}
				else 
				{
					state_ = INITIAL;
					connection_.show_diagnostics();
				}
				bind_counter_ = 1;
				static_data_.clear();
			}
			
			bool statement::is_valid() const
			{
				return stmt_ != nullptr;
			}
			
			statement::operator sqlite3_stmt*()
			{
				return stmt_;
			}			

			statement_ptr statement_factory(connection& con)
			{
				auto ptr = std::make_shared<statement>(con);
				return std::static_pointer_cast<interface_statement>(ptr->shared_from_this());
			}

		}
	}
}

