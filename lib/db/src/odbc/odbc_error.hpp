/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#if ! defined CYNG_DB_ODBC_ERROR_HPP && CYNG_ODBC_INSTALLED
#define CYNG_DB_ODBC_ERROR_HPP

#include <boost/system/error_code.hpp>
			
namespace cyng
{
	namespace db 
	{	
		namespace odbc
		{
			namespace error 
			{
				enum odbc_errors
				{
					STATE_00000,	//!<	Success
					STATE_01000,	//!<	General warning (All ODBC functions except: SQLAllocEnv)
					STATE_01001,	//!<	Cursor operation conflict
					STATE_01002,	//!<	Disconnect error (SQLDisconnect)
					STATE_01003,	//!<	NULL value eliminated in set function
					STATE_01004,	//!<	String data, right-truncated (SQLBrowseConnect)
					STATE_01006,	//!<	Privilege not revoked (SQLExecDirect)
					STATE_01S00,	//!<	Invalid connection string attribute (SQLBrowseConnect)
					STATE_01S01,	//!<	Error in row
					STATE_01S02,	//!<	Option value changed
					STATE_01S03,	//!<	No rows updated or deleted
					STATE_01S04,	//!<	More than one row updated or deleted
					STATE_01S05,	//!<	Cancel treated as FreeStmt / Close
					STATE_01S06,	//!<	Attempt to fetch before the result returned the first rowset
					STATE_07001,	//!<	Wrong number of parameters
					STATE_07006,	//!<	Restricted data type attribute violation
					STATE_07S01,	//!<	Invalid use of default paramater
					STATE_08001,	//!<	Unable to connect to data source
					STATE_08002,	//!<	Connection in use
					STATE_08003,	//!<	Connection not open
					STATE_08004,	//!<	Data source rejected establishment of connection
					STATE_08007,	//!<	Connection failure during transaction
					STATE_08S01,	//!<	Communication link failure
					STATE_21S01,	//!<	Insert value list does not match column list
					STATE_21S02,	//!<	Degree of derived table does not match column list
					STATE_22001,	//!<	String data right truncation
					STATE_22002,	//!<	Indicator variable required but not supplied
					STATE_22003,	//!<	Numeric value out of range
					STATE_22005,	//!<	Error in assignment
					STATE_22008,	//!<	Datetime field overflow
					STATE_22012,	//!<	Division by zero
					STATE_22026,	//!<	String data, length mismatch
					STATE_23000,	//!<	Integrity constraint violation
					STATE_24000,	//!<	Invalid cursor state
					STATE_25000,	//!<	Invalid transaction state
					STATE_28000,	//!<	Invalid authorization specification

					STATE_34000,	//!<	Invalid cursor name
					STATE_37000,	//!<	Syntax error or access violation
					STATE_3C000,	//!<	Duplicate cursor name
					STATE_3D000,	//!<	Invalid catalog name
					STATE_3F000,	//!<	Invalid schema name

					STATE_40001,	//!<	Serialization failure
					STATE_40002,	//!<	Integrity constraint violation
					STATE_40003,	//!<	Statement completion unknown
					STATE_42000,	//!<	Syntax error or access violation
					STATE_42S01,	//!<	Base table or view already exists
					STATE_42S02,	//!<	Base table or view not found
					STATE_42S11,	//!<	Index already exists
					STATE_42S12,	//!<	Index not found
					STATE_42S21,	//!<	Column already exists
					STATE_42S22,	//!<	Column not found
					STATE_44000,	//!<	WITH CHECK OPTION violation

					STATE_70100,	//!<	Operation aborted
					STATE_IM001,	//!<	Driver does not support this function
					STATE_IM002,	//!<	Data source name not found and no default driver specified
					STATE_IM003,	//!<	Specified driver could not be loaded
					STATE_IM004,	//!<	Driver's SQLAllocEnv failed
					STATE_IM005,	//!<	Driver's SQLAllocConnect failed
					STATE_IM006,	//!<	Driver's SQLSetConnectOption failed
					STATE_IM007,	//!<	No data source or driver specified; dialog prohibited
					STATE_IM008,	//!<	Dialog failed
					STATE_IM009,	//!<	Unable to load translation DLL
					STATE_IM010,	//!<	Data source name too long
					STATE_IM011,	//!<	Driver name too long
					STATE_IM012,	//!<	DRIVER keyword syntax error
					STATE_IM013,	//!<	Trace file error
					STATE_S0001,	//!<	Base table or view already exists
					STATE_S0002,	//!<	Base table not found
					STATE_S0011,	//!<	Index already exists
					STATE_S0012,	//!<	Index not found
					STATE_S0021,	//!<	Column already exists
					STATE_S0022,	//!<	Column not found
					STATE_S0023,	//!<	No default for column
					STATE_S1000,	//!<	General error
					STATE_S1001,	//!<	Memory allocation failure
					STATE_S1002,	//!<	Invalid column number
					STATE_S1003,	//!<	Program type out of range
					STATE_S1004,	//!<	SQL data type out of range
					STATE_S1008,	//!<	Operation canceled
					STATE_S1009,	//!<	Invalid argument value
					STATE_S1010,	//!<	Function sequence error
					STATE_S1011,	//!<	Operation invalid at this time
					STATE_S1012,	//!<	Invalid transaction operation code specified
					STATE_S1015,	//!<	No cursor name available
					STATE_S1090,	//!<	Invalid string or buffer length
					STATE_S1091,	//!<	Descriptor type out of range
					STATE_S1092,	//!<	Option type out of range
					STATE_S1093,	//!<	Invalid parameter number
					STATE_S1094,	//!<	Invalid scale value
					STATE_S1095,	//!<	Function type out of range
					STATE_S1096,	//!<	Information type out of range
					STATE_S1097,	//!<	Column type out of range
					STATE_S1098,	//!<	Scope type out of range
					STATE_S1099,	//!<	Nullable type out of range
					STATE_S1100,	//!<	Uniqueness option type out of range
					STATE_S1101,	//!<	Accuracy option type out of range
					STATE_S1103,	//!<	Direction option out of range
					STATE_S1104,	//!<	Invalid precision value
					STATE_S1105,	//!<	Invalid parameter type
					STATE_S1106,	//!<	Fetch type out of range (SQLExtendedFetch)
					STATE_S1107,	//!<	Row value out of range (SQLExtendedFetch)
					STATE_S1108,	//!<	Concurrency option out of range (SQLSetScrollOptions)
					STATE_S1109,	//!<	Invalid cursor position
					STATE_S1110,	//!<	Invalid driver completion
					STATE_S1111,	//!<	Invalid bookmark value
					STATE_S1C00,	//!<	Driver not capable
					STATE_S1DE0,	//!<	No data at execution values pending
					STATE_S1T00,	//!<	Timeout expired
					STATE_HY000,	//!<	General error
					STATE_HY009,	//!<	Invalid use of null pointer
					STATE_HY010,	//!<	Function sequence error
					STATE_UNKNWON,	//!<	unknown error
				};
				
				//	ODBC error
				extern const boost::system::error_category& get_odbc_category();

				//	The identity of an error_category-derived object is determined by its address.
				static const boost::system::error_category& odbc_category = get_odbc_category();
				
			}
		}
	}
}

#include <boost/type_traits/integral_constant.hpp>	//	provides true_type

namespace boost		{
	namespace system	{

		template<>
		struct is_error_code_enum<cyng::db::odbc::error::odbc_errors>
			: true_type{};


	}	//	namespace system
}	//	namespace boost

namespace cyng
{
	namespace db 
	{	
		namespace odbc
		{
			namespace error 
			{
				inline boost::system::error_code make_error_code(int e)	
				{
					return boost::system::error_code(
						static_cast<int>(e), get_odbc_category());
				}

				//	implementation see error.cpp
				const boost::system::error_category& get_odbc_category();
				
			}
		}
	}
}

#endif	//	CYNG_DB_ODBC_ERROR_HPP
