/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Sylko Olzscher
 *
 */

#if defined(CYNG_ODBC_INSTALLED)
#ifndef CYNG_DB_ODBC_DIAG_HPP
#define CYNG_DB_ODBC_DIAG_HPP

#include "odbc_defs.h"
#include "odbc_error.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <boost/assert.hpp>
#include <boost/algorithm/string/predicate.hpp>

namespace cyng
{
	namespace db	
	{
		namespace odbc	
		{

			struct diagnostic_fields	
			{
				/// SQLGetDiagRec fields
				const std::string message_;
				const std::string state_;
				const SQLINTEGER native_error_;
				diagnostic_fields(SQLCHAR* msg, std::size_t msg_size, SQLCHAR* state, std::size_t state_size, SQLINTEGER native_error)
					: message_(msg, msg + msg_size)
					, state_(state, state + state_size)
					, native_error_(native_error)
				{
					std::stringstream ss;
					dump(ss);
#ifdef _WIN32
					::OutputDebugString(ss.str().c_str());
#endif
					std::cerr
						<< ss.str()
						<< std::endl
						;
				}
				void dump(std::ostream & os)	{
					os
						<< native_error_
						<< " ["
						<< state_
						<< "] "
						<< message_
						;
				}

				error::odbc_errors get_code() const	{
					if (boost::algorithm::equals(state_, "00000"))	return error::STATE_00000;
					else if (boost::algorithm::equals(state_, "01000"))	return error::STATE_01000;
					else if (boost::algorithm::equals(state_, "01001"))	return error::STATE_01001;
					else if (boost::algorithm::equals(state_, "01002"))	return error::STATE_01002;
					else if (boost::algorithm::equals(state_, "01003"))	return error::STATE_01003;
					else if (boost::algorithm::equals(state_, "01004"))	return error::STATE_01004;
					else if (boost::algorithm::equals(state_, "01006"))	return error::STATE_01006;
					else if (boost::algorithm::equals(state_, "01S00"))	return error::STATE_01S00;
					else if (boost::algorithm::equals(state_, "01S01"))	return error::STATE_01S01;
					else if (boost::algorithm::equals(state_, "01S02"))	return error::STATE_01S02;
					else if (boost::algorithm::equals(state_, "01S03"))	return error::STATE_01S03;

					else if (boost::algorithm::equals(state_, "40001"))	return error::STATE_40001;
					else if (boost::algorithm::equals(state_, "40002"))	return error::STATE_40002;
					else if (boost::algorithm::equals(state_, "40003"))	return error::STATE_40003;
					else if (boost::algorithm::equals(state_, "42000"))	return error::STATE_42000;
					else if (boost::algorithm::equals(state_, "42S01"))	return error::STATE_42S01;
					else if (boost::algorithm::equals(state_, "42S02"))	return error::STATE_42S02;
					else if (boost::algorithm::equals(state_, "42S11"))	return error::STATE_42S11;
					else if (boost::algorithm::equals(state_, "42S12"))	return error::STATE_42S12;
					else if (boost::algorithm::equals(state_, "42S21"))	return error::STATE_42S21;
					else if (boost::algorithm::equals(state_, "42S22"))	return error::STATE_42S22;
					else if (boost::algorithm::equals(state_, "44000"))	return error::STATE_44000;

					else if (boost::algorithm::equals(state_, "HY000"))	return error::STATE_HY000;
					else if (boost::algorithm::equals(state_, "HY009"))	return error::STATE_HY009;
					else if (boost::algorithm::equals(state_, "HY010"))	return error::STATE_HY010;
					else return error::STATE_UNKNWON;
				}

			};

			/**
			*	Utility class providing functionality for retrieving ODBC diagnostic
			*	records. Diagnostics object must be created with corresponding handle
			*	as constructor argument. During construction, diagnostic records fields
			*	are populated and the object is ready for querying.
			*/
			template < typename H, SQLSMALLINT handle_N >
			class diagnostics	{
			public:
				void run(H handle)	{
					SQLSMALLINT	index = 0, sql_msg_len;
					SQLRETURN rc;
					SQLCHAR	sql_state[ SQL_SQLSTATE_SIZE + 1 ];
					SQLCHAR	sql_msg[ SQL_MAX_MESSAGE_LENGTH + 1 ];
					SQLINTEGER    native_error;
					while ((rc = ::SQLGetDiagRec(handle_N
						, handle
						, ++index
						, sql_state
						, &native_error
						, sql_msg
						, sizeof(sql_msg)
						, &sql_msg_len)) != SQL_NO_DATA)	{
							
						diagnostic_fields df(sql_msg, sql_msg_len, sql_state, SQL_SQLSTATE_SIZE, native_error);
						messages_.push_back(df);
					}
				}

				inline std::vector< diagnostic_fields > const&
					get_messages()	{
						return messages_;
					}

				inline bool empty() const	{
					return messages_.empty();
				}

			private:
				std::vector< diagnostic_fields >	messages_;
			};

			//	predefined diagnostic types
			typedef diagnostics< SQLHENV, SQL_HANDLE_ENV >		environment_diagnostics;
			typedef diagnostics< SQLHDBC, SQL_HANDLE_DBC >		connection_diagnostics;
			typedef diagnostics< SQLHSTMT, SQL_HANDLE_STMT >	statement_diagnostics;
			typedef diagnostics< SQLHDESC, SQL_HANDLE_DESC >	descriptor_diagnostics;

		}	//	odbc
	}	//	db
}	//	cyng

#endif	//	CYNG_DB_ODBC_DIAG_HPP
#endif
