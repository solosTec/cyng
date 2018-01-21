/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#if ! defined CYNG_DB_ODBC_HANDLE_HPP && CYNG_ODBC_INSTALLED
#define CYNG_DB_ODBC_HANDLE_HPP

#include "odbc_defs.h"
#include "odbc_exception.hpp"
#include <string>
#include <boost/assert.hpp>

namespace cyng	
{
	namespace db	
	{
		namespace odbc	
		{
			namespace detail	
			{

				/*	@brief for debugging purposes
				 *	Helper to determine the ODBC handle type
				 */
				template < int T >
				struct handle_traits	
				{
//						static_assert(false, "unknown ODBC handle type");
// 					static const int null_value = 0;
                    static constexpr SQLHANDLE null_value = nullptr;
					static std::string handle_type()	
					{
						return "not supported";
					}
				};

				template <>
				struct handle_traits < SQL_HANDLE_ENV >	
				{
// 					static const int null_value = { SQL_NULL_HENV };
                    static constexpr SQLHANDLE null_value = { SQL_NULL_HENV };
					static std::string handle_type()	
					{
						return "SQL_HANDLE_ENV";
					}
				};

				template <>
				struct handle_traits < SQL_HANDLE_DBC >	
				{
// 					static const int null_value = { SQL_NULL_HDBC };
                    static constexpr SQLHANDLE null_value = { SQL_NULL_HDBC };
					static std::string handle_type()	
					{
						return "SQL_HANDLE_DBC";
					}
				};

				template <>
				struct handle_traits < SQL_HANDLE_STMT >	
				{
// 					static const int null_value = { SQL_NULL_HSTMT };
                    static constexpr SQLHANDLE null_value = { SQL_NULL_HSTMT };
					static std::string handle_type()	
					{
						return "SQL_HANDLE_STMT";
					}
				};

				template <>
				struct handle_traits < SQL_HANDLE_DESC >	
				{
// 					static const int null_value = { SQL_NULL_HDESC };
                    static constexpr SQLHANDLE null_value = { SQL_NULL_HDESC };
					static std::string handle_type()	
					{
						return "SQL_HANDLE_DESC";
					}
				};
			}	//	detail


			/**
			 *	ODBC handle class template
			 */
			template < typename H, SQLSMALLINT handle_N >
			class odbc_handle	
			{
			public:
				odbc_handle(SQLHDBC h)
#ifdef _MSC_VER
					: handle_(detail::handle_traits< handle_N >::null_value)	
#else
					: handle_((H)detail::handle_traits< handle_N >::null_value)	
#endif
					{
					const SQLRETURN rc = ::SQLAllocHandle(handle_N, h, &handle_);
					BOOST_ASSERT(is_ok(rc));
					if (!is_ok(rc))	
					{
						BOOST_THROW_EXCEPTION(odbc_error("SQLAllocHandle"));
					}
				}

				odbc_handle(odbc_handle&& other)
					: handle_(std::forward< H >(other.handle_))
				{
#ifdef _MSC_VER
					other.handle_ = detail::handle_traits< handle_N >::null_value;
#else
					other.handle_ = (H)detail::handle_traits< handle_N >::null_value;
#endif
				}

				odbc_handle(H&& h)
					: handle_(std::forward< H >(h))
				{
					h = detail::handle_traits< handle_N >::null_value;
				}

				//	To prevent copying, but not really needed because 
				//	if a move constructor or move-assignment operator is explicitly declared, then
				//	no copy constructor is automatically generated.
				odbc_handle(odbc_handle const& other) = delete;

				//	RIIA
				~odbc_handle()	
				{
					if (handle_ != detail::handle_traits< handle_N >::null_value)	
					{
						const SQLRETURN rc = ::SQLFreeHandle(handle_N, handle_);
						BOOST_ASSERT(SQL_SUCCEEDED(rc));
					}
				}

				/**
				 *	@return a textual description of this ODBC handle type
				 */
				static std::string handle_type()	
				{
					return detail::handle_traits< handle_N >::handle_type();
				}

				/**
				 *	Const conversion operator into reference to native type.
				 */
				operator const H& () const	
				{
					return get_handle();
				}

				/**
				 *	@return const reference to native type.
				 */
				const H& get_handle() const	
				{
					return handle_;
				}

				bool is_valid() const	
				{
					return handle_ != detail::handle_traits< handle_N >::null_value;
				}

			private:
				H handle_;

			};

			typedef odbc_handle< SQLHSTMT, SQL_HANDLE_ENV > odbc_environment_handle;	//	SQL_HANDLE_ENV = 1
			typedef odbc_handle< SQLHDESC, SQL_HANDLE_DBC > odbc_db_connection_handle;	//	SQL_HANDLE_DBC = 2
			typedef odbc_handle< SQLHSTMT, SQL_HANDLE_STMT > odbc_statement_handle;		//	SQL_HANDLE_STMT = 3
			typedef odbc_handle< SQLHDESC, SQL_HANDLE_DESC > odbc_descriptor_handle;	//	SQL_HANDLE_DESC = 4

			//	specialization for environment handle
			template <>
			class odbc_handle <SQLHSTMT, SQL_HANDLE_ENV>	
			{
			public:
				odbc_handle()	
				{
//					using namespace cyng::db::odbc::error;

					if (!is_ok(::SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &handle_)))	
					{
						BOOST_THROW_EXCEPTION(odbc_error("SQLAllocHandle(SQL_HANDLE_ENV)"));
					}
					
					//	ODBC 3 support
					if (!is_ok(::SQLSetEnvAttr(handle_, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0)))	
					{
						BOOST_THROW_EXCEPTION(odbc_error("SQLSetEnvAttr"));
					}
				}

				~odbc_handle()	
				{
					const SQLRETURN rc = ::SQLFreeHandle(SQL_HANDLE_ENV, handle_);
					BOOST_ASSERT(is_ok(rc));
				}

				const SQLHSTMT& get_handle() const	
				{
					return handle_;
				}

				bool is_valid() const	
				{
					return handle_ != detail::handle_traits< SQL_HANDLE_ENV >::null_value;
				}

			private:
				SQLHSTMT handle_;
			};

		}	//	odbc
	}	//	db
}	//	cyng

#endif	//	CYNG_DB_ODBC_HANDLE_HPP

