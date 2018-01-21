/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#include "odbc_error.hpp"
			
namespace cyng
{
	namespace db 
	{	
		namespace odbc
		{
			namespace error 
			{
				namespace detail
				{
					class odbc_errors_category_impl
					: public boost::system::error_category
					{
					public:
						//	return a string identifying the category.
						virtual const char* name() const BOOST_SYSTEM_NOEXCEPT 
						{
							return "ODBC";
						}
						//	Converts an error value into a string that describes the error.
						virtual std::string message(int ev) const	
						{
							switch (ev)	
							{
								case STATE_00000: return "Success";
								case STATE_01000: return "[01000] General warning";
								case STATE_01001: return "[01001] Cursor operation conflict";
								case STATE_01002: return "[01002] Disconnect error";
								case STATE_01003: return "[01003] NULL value eliminated in set function";
								case STATE_01004: return "[01004] String data, right-truncated";
								case STATE_01006: return "Privilege not revoked";
								case STATE_01S00: return "Invalid connection string attribute";
								case STATE_01S01: return "Error in row";
								case STATE_01S02: return "Option value changed";
								case STATE_01S03: return "No rows updated or deleted";
								case STATE_01S04: return "More than one row updated or deleted";
								case STATE_01S05: return "Cancel treated as FreeStmt / Close";
								case STATE_01S06: return "Attempt to fetch before the result returned the first rowset";
								case STATE_07001: return "Wrong number of parameters";
								case STATE_07006: return "Restricted data type attribute violation";
								case STATE_07S01: return "Invalid use of default paramater";
								case STATE_08001: return "Unable to connect to data source";
								case STATE_08002: return "Connection in use";
								case STATE_08003: return "Connection not open";
								case STATE_08004: return "Data source rejected establishment of connection";
								case STATE_08007: return "Connection failure during transaction";
								case STATE_08S01: return "Communication link failure";
								case STATE_21S01: return "Insert value list does not match column list";
								case STATE_21S02: return "Degree of derived table does not match column list";
								case STATE_22001: return "String data right truncation";
								case STATE_22002: return "Indicator variable required but not supplied";
								case STATE_22003: return "Numeric value out of range";
								case STATE_22005: return "Error in assignment";
								case STATE_22008: return "Datetime field overflow";
								case STATE_22012: return "Division by zero";
								case STATE_22026: return "String data, length mismatch";
								case STATE_23000: return "Integrity constraint violation";
								case STATE_24000: return "Invalid cursor state";
								case STATE_25000: return "Invalid transaction state";
								case STATE_28000: return "Invalid authorization specification";

								case STATE_34000: return "Invalid cursor name";
								case STATE_37000: return "Syntax error or access violation";
								case STATE_3C000: return "Duplicate cursor name";
								case STATE_3D000: return "Invalid catalog name";
								case STATE_3F000: return "Invalid schema name";

								case STATE_40001: return "Serialization failure";
								case STATE_40002: return "Integrity constraint violation";
								case STATE_40003: return "Statement completion unknown";
								case STATE_42000: return "Syntax error or access violation";
								case STATE_42S01: return "Base table or view already exists";
								case STATE_42S02: return "Base table or view not found";
								case STATE_42S11: return "Index already exists";
								case STATE_42S12: return "Index not found";
								case STATE_42S21: return "Column already exists";
								case STATE_42S22: return "Column not found";
								case STATE_44000: return "WITH CHECK OPTION violation";

								case STATE_70100: return "Operation aborted";
								case STATE_IM001: return "Driver does not support this function";
								case STATE_IM002: return "Data source name not found and no default driver specified";
								case STATE_IM003: return "Specified driver could not be loaded";
								case STATE_IM004: return "Driver's SQLAllocEnv failed";
								case STATE_IM005: return "Driver's SQLAllocConnect failed";
								case STATE_IM006: return "Driver's SQLSetConnectOption failed";
								case STATE_IM007: return "No data source or driver specified; dialog prohibited";
								case STATE_IM008: return "Dialog failed";
								case STATE_IM009: return "Unable to load translation DLL";
								case STATE_IM010: return "Data source name too long";
								case STATE_IM011: return "Driver name too long";
								case STATE_IM012: return "DRIVER keyword syntax error";
								case STATE_IM013: return "Trace file error";
								case STATE_S0001: return "Base table or view already exists";
								case STATE_S0002: return "Base table not found";
								case STATE_S0011: return "Index already exists";
								case STATE_S0012: return "Index not found";
								case STATE_S0021: return "Column already exists";
								case STATE_S0022: return "Column not found";
								case STATE_S0023: return "No default for column";
								case STATE_S1000: return "General error";
								case STATE_S1001: return "Memory allocation failure";
								case STATE_S1002: return "Invalid column number";
								case STATE_S1003: return "Program type out of range";
								case STATE_S1004: return "SQL data type out of range";
								case STATE_S1008: return "Operation canceled";
								case STATE_S1009: return "Invalid argument value";
								case STATE_S1010: return "Function sequence error";
								case STATE_S1011: return "Operation invalid at this time";
								case STATE_S1012: return "Invalid transaction operation code specified";
								case STATE_S1015: return "No cursor name available";
								case STATE_S1090: return "Invalid string or buffer length";
								case STATE_S1091: return "Descriptor type out of range";
								case STATE_S1092: return "Option type out of range";
								case STATE_S1093: return "Invalid parameter number";
								case STATE_S1094: return "Invalid scale value";
								case STATE_S1095: return "Function type out of range";
								case STATE_S1096: return "Information type out of range";
								case STATE_S1097: return "Column type out of range";
								case STATE_S1098: return "Scope type out of range";
								case STATE_S1099: return "Nullable type out of range";
								case STATE_S1100: return "Uniqueness option type out of range";
								case STATE_S1101: return "Accuracy option type out of range";
								case STATE_S1103: return "Direction option out of range";
								case STATE_S1104: return "Invalid precision value";
								case STATE_S1105: return "Invalid parameter type";
								case STATE_S1106: return "Fetch type out of range";
								case STATE_S1107: return "Row value out of range";
								case STATE_S1108: return "Concurrency option out of range";
								case STATE_S1109: return "Invalid cursor position";
								case STATE_S1110: return "Invalid driver completion";
								case STATE_S1111: return "Invalid bookmark value";
								case STATE_S1C00: return "Driver not capable";
								case STATE_S1DE0: return "[S1DE0] No data at execution values pending";
								case STATE_S1T00: return "[S1T00] Timeout expired";

								case STATE_HY000:	return "[HY000] General error";
								case STATE_HY009:	return "[HY009] Invalid use of null pointer";
								case STATE_HY010:	return "[HY010] Function sequence error";
								case STATE_UNKNWON:	return "Unknown error";
								default:
									break;
								}
								return "[]";
						}
					};
				}	//	detail
				
				const boost::system::error_category&
				get_odbc_category()	
				{
					static detail::odbc_errors_category_impl instance;
					return instance;
				}
			}
		}
	}
}


