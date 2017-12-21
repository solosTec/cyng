/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/sql/dialect.h>
#include <cyng/intrinsics/traits/tag.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <sstream>

namespace cyng 
{
	namespace sql 
	{
		dialect get_dialect(std::string const& name)
		{
			if (boost::algorithm::iequals(name, "MYSQL"))	
			{
				return MYSQL;
			}
			else if (boost::algorithm::iequals(name, "SQLITE"))	
			{
				return SQLITE;
			}
			else if (boost::algorithm::iequals(name, "ORACLE"))	
			{
				return ORACLE;
			}
			else if (boost::algorithm::iequals(name, "ACCESS") || boost::algorithm::iequals(name, "MSACCESS"))	
			{
				return ACCESS;
			}
			else if (boost::algorithm::iequals(name, "SQLSERVER") || boost::algorithm::iequals(name, "MSSQLServer") || boost::algorithm::iequals(name, "MSSQL"))	
			{
				return SQLSERVER;
			}
			else if (boost::algorithm::iequals(name, "POSTGRESQL") || boost::algorithm::iequals(name, "PGSQL"))
			{
				return POSTGRESQL;
			}
			
			return GENERIC;
		}
		
		bool has_feature(dialect dia, feature feat)
		{
			switch (dia)
			{
				case GENERIC:
					switch (feat)
					{
						case DATE_TIME_SUPPORT:	return true;
						default:
							break;
					}
					break;
				case MYSQL:
					switch (feat)
					{
						case IF_NOT_EXISTS:	return true;
						case DATE_TIME_SUPPORT:	return true;
						default:
							break;
					}
					break;
				case SQLITE:
					switch (feat)
					{
						case IF_NOT_EXISTS:	return true;
						case DATE_TIME_SUPPORT:	return false;
						default:
							break;
					}
					break;
				case ORACLE:
					switch (feat)
					{
						case DATE_TIME_SUPPORT:	return true;
						default:
							break;
					}
					break;
				case ACCESS:
					switch (feat)
					{
						case DATE_TIME_SUPPORT:	return true;
						default:
							break;
					}
					break;
				case SQLSERVER:
					switch (feat)
					{
						case DATE_TIME_SUPPORT:	return true;
						default:
							break;
					}
					break;
				case POSTGRESQL:
					switch (feat)
					{
						//	with some rough edges
						case IF_NOT_EXISTS:	return true;
						case DATE_TIME_SUPPORT:	return true;
						default:
							break;
					}
					break;
				default:
					break;
			}
			return false;
		}
		
		std::string get_field_type(dialect d, std::size_t code, std::size_t width)
		{
			switch (d)
			{
				case MYSQL:
					switch (code)
					{
					case TC_BOOL:			return "TINYINT(1)";	//	BOOL is a synonym
					case TC_CHAR:
					case TC_INT8:			return "TINYINT  UNSIGNED";		//	unsigned 
					case TC_UINT8:			return "TINYINT";	//	-128 to 127 (1 byte)
					case TC_INT16:			return "SMALLINT";	//	-32.768 to 32.767 (16 bit)
					case TC_UINT16:			return "SMALLINT UNSIGNED";		//	unsigned 
					case TC_INT32:			return "INT";		//	32 bit
					case TC_UINT32:			return "INT UNSIGNED";			//	0 to 4.294.967.295
					case TC_INT64:			return "BIGINT";	//	-9.223.372.036.854.775.808 to 9.223.372.036.854.775.807 (8 bytes)
					case TC_UINT64:			return "BIGINT UNSIGNED";		//	0 to 18.446.744.073.709.551.615 == UINT64_MAX
					case TC_FLOAT:			return "FLOAT";		//!<	float
					case TC_DOUBLE:			return "DOUBLE";	//!<	double
					case TC_FLOAT80:		return "DOUBLE";	//!<	double
					case TC_STRING:
					{
						BOOST_ASSERT_MSG(width != 0, "column width is null");
						std::ostringstream ostream;
						ostream << "VARCHAR(" << width << ")";
						return ostream.str();
					}
					case TC_TIME_POINT:		return "DATETIME";	//	'1000-01-01 00:00:00' to '9999-12-31 23:59:59' ('YYYY-MM-DD HH:MM:SS')
					case TC_NANO_SECOND:	return "BIGINT";	
					case TC_MICRO_SECOND:	return "BIGINT";	
					case TC_MILLI_SECOND:	return "BIGINT";	
					case TC_SECOND:			return "BIGINT";	
					case TC_MINUTE:			return "BIGINT";	
					case TC_HOUR:			return "BIGINT";	
// 					case CYY_DAYS:			return "BIGINT";	
					case TC_VERSION:		return "INT UNSIGNED";
					case TC_REVISION:		return "BIGINT UNSIGNED";
					case TC_CODE:			return "INT UNSIGNED";
					case TC_BUFFER:			return "BLOB";
// 					case CYY_INDEX:			return "BIGINT UNSIGNED";
// 					case CYY_DIFF:			return "BIGINT";
	// 				case CYY_RGB_COLOR8:	return "rgb8";
	// 				case CYY_RGB_COLOR16:	return "rgb16";
					case TC_MAC48:			return "CHAR(17)";
					case TC_MAC64:			return "CHAR(19)";

					case TC_ATTR_MAP:		return "TEXT";
					case TC_PARAM_MAP:		return "TEXT";
					case TC_ATTR:			return "TEXT";	//	max_allowed_packet (1048576 bytes)
					case TC_PARAM:			return "TEXT";
					case TC_TUPLE:			return "TEXT";
					case TC_VECTOR:			return "TEXT";
					case TC_SET:			return "TEXT";
	// 				case CYY_LOCKABLE:		return "lock";
	// 				case CYY_TABLE:			return "table";
					
					case TC_UUID:	
						BOOST_ASSERT(width == 36 || width == -1);
						return "CHAR(36)";	//	string representation of a uuid is hhhhhhhh-hhhh-hhhh-hhhh-hhhhhhhhhhhh (36)
					case TC_FS_PATH:	
						BOOST_ASSERT(width == 1024 || width == -1);
						return "VARCHAR(1024)";
	//				case CYY_BOOST_ERROR:	return "ec";
// 					case CYY_BOOST_IP_ADDRESS:		
// 						BOOST_ASSERT(width == 40 || width == -1);
// 						return "CHAR(40)";	//	!<	IPv4 and IPv6 as string
// 					case CYY_BOOST_TCP_ENDPOINT:	
// 						BOOST_ASSERT(width == 48 || width == -1);
// 						return "CHAR(48)";	//	address and port as string e.g. [ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff]:65000
	//				case CYY_BOOST_TRIBOOL:	return "tribool";
	//				case CYY_ARRAY:			return "[]";
						default:
							break;
					}
					return "BLOB";
					
				case SQLITE:
					switch (code)
					{
					case TC_BOOL:			return "INT";	//!<	1 == true, 0 == false
					case TC_CHAR:
					case TC_INT8:			return "INT";	
					case TC_UINT8:			return "INT";	
					case TC_INT16:			return "INT";	
					case TC_UINT16:			return "INT";	
					case TC_INT32:			return "INT";	
					case TC_UINT32:			return "INT";	
					case TC_INT64:			return "INT";	
					case TC_UINT64:			return "INT";	
					case TC_FLOAT:			return "FLOAT";	
					case TC_DOUBLE:			return "FLOAT";	
					case TC_FLOAT80:		return "FLOAT";	
					case TC_STRING:			return "TEXT";
					case TC_TIME_POINT:		return "FLOAT";	//	stored as julianday (select julianday('2011-03-20', 'utc');)
					case TC_NANO_SECOND:	return "INT";	
					case TC_MICRO_SECOND:	return "INT";	
					case TC_MILLI_SECOND:	return "INT";	
					case TC_SECOND:			return "INT";	
					case TC_MINUTE:			return "INT";	
					case TC_HOUR:			return "INT";	
// 					case CYY_DAYS:			return "INT";	
					case TC_VERSION:		return "INT";
					case TC_REVISION:		return "INT";
					case TC_CODE:			return "INT";
					case TC_BUFFER:			return "BLOB";
// 					case CYY_INDEX:			return "BIGINT UNSIGNED";
// 					case CYY_DIFF:			return "BIGINT";
	// 				case CYY_RGB_COLOR8:	return "rgb8";
	// 				case CYY_RGB_COLOR16:	return "rgb16";
					case TC_MAC48:			return "TEXT";
					case TC_MAC64:			return "TEXT";

					case TC_ATTR_MAP:		return "TEXT";
					case TC_PARAM_MAP:		return "TEXT";
					case TC_ATTR:			return "TEXT";	//	max_allowed_packet (1048576 bytes)
					case TC_PARAM:			return "TEXT";
					case TC_TUPLE:			return "TEXT";
					case TC_VECTOR:			return "TEXT";
					case TC_SET:			return "TEXT";
	// 				case CYY_LOCKABLE:		return "lock";
	// 				case CYY_TABLE:			return "table";
					
					case TC_UUID:			return "TEXT";
					case TC_FS_PATH:		return "TEXT";
	//				case CYY_BOOST_ERROR:	return "ec";
// 					case CYY_BOOST_IP_ADDRESS:		
// 						BOOST_ASSERT(width == 40 || width == -1);
// 						return "CHAR(40)";	//	!<	IPv4 and IPv6 as string
// 					case CYY_BOOST_TCP_ENDPOINT:	
// 						BOOST_ASSERT(width == 48 || width == -1);
// 						return "CHAR(48)";	//	address and port as string e.g. [ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff]:65000
	//				case CYY_BOOST_TRIBOOL:	return "tribool";
	//				case CYY_ARRAY:			return "[]";
						default:
							break;
					}
					return "BLOB";
					
				case ORACLE:
					switch (code)
					{
					case TC_BOOL:			return "BIT(1)";	//!<	1 == true, 0 == false
					case TC_CHAR:
					case TC_INT8:			return "TINYINT";	
					case TC_UINT8:			return "UNSIGNED TINYINT";	
					case TC_INT16:			return "SMALLINT";
					case TC_UINT16:			return "UNSIGNED SMALLINT";	
					case TC_INT32:			return "INTEGER";
					case TC_UINT32:			return "UNSIGNED INTEGER";
					case TC_INT64:			return "BIGINT";
					case TC_UINT64:			return "UNSIGNED BIGINT";
					case TC_FLOAT:			return "REAL";
					case TC_DOUBLE:			return "DOUBLE";
					case TC_FLOAT80:		return "DOUBLE";
					case TC_STRING:
						{
							BOOST_ASSERT_MSG(width != 0, "column width is null");
							std::ostringstream ostream;
							ostream << "VARCHAR(" << width << ")";
							return ostream.str();
						}
						
					case TC_TIME_POINT:		return "DATETIME";	//	//	'1000-01-01 00:00:00' to '9999-12-31 23:59:59' ('YYYY-MM-DD HH:MM:SS')
					case TC_NANO_SECOND:	return "BIGINT";	
					case TC_MICRO_SECOND:	return "BIGINT";	
					case TC_MILLI_SECOND:	return "BIGINT";	
					case TC_SECOND:			return "BIGINT";	
					case TC_MINUTE:			return "BIGINT";	
					case TC_HOUR:			return "BIGINT";	
// 					case TC_DAYS:			return "BIGINT";	
					case TC_VERSION:		return "UNSIGNED INTEGER";
					case TC_REVISION:		return "UNSIGNED BIGINT";
					case TC_CODE:			return "UNSIGNED INTEGER";
					case TC_BUFFER:			return "BLOB";
// 					case CYY_INDEX:			return "BIGINT UNSIGNED";
// 					case CYY_DIFF:			return "BIGINT";
	// 				case CYY_RGB_COLOR8:	return "rgb8";
	// 				case CYY_RGB_COLOR16:	return "rgb16";
					case TC_MAC48:			return "CHAR(17)";
					case TC_MAC64:			return "CHAR(19)";

					case TC_ATTR_MAP:		return "VARCHAR(2048)";
					case TC_PARAM_MAP:		return "VARCHAR(2048)";
					case TC_ATTR:			return "VARCHAR(512)";	
					case TC_PARAM:			return "VARCHAR(512)";
					case TC_TUPLE:			return "VARCHAR(2048)";
					case TC_VECTOR:			return "VARCHAR(2048)";
					case TC_SET:			return "VARCHAR(2048)";
	// 				case CYY_LOCKABLE:		return "lock";
	// 				case CYY_TABLE:			return "table";
					
					case TC_UUID:			return "CHAR(36)";
					case TC_FS_PATH:		return "CHAR(1024)";
						default:
							break;
					}
					return "BLOB";
					
				case ACCESS:
					//	ToDo:
					break;
					
				case SQLSERVER:
					switch (code)
					{
					case TC_BOOL:			return "BIT";	//!<	Allows 0, 1, or NULL
					case TC_CHAR:
					case TC_INT8:			return "SMALLINT";	//	no native representation
					case TC_UINT8:			return "TINYINT";	//	Allows whole numbers from 0 to 255
					case TC_INT16:			return "SMALLINT";	//	-32.768 to 32.767 (16 bit)
					case TC_UINT16:			return "INTEGER";	//	no native support 
					case TC_INT32:			return "INT";		//	Allows whole numbers between -2,147,483,648 and 2,147,483,647 (4 bytes)
					case TC_UINT32:			return "INTEGER";	//	no native support 
					case TC_INT64:			return "BIGINT";	//	-9.223.372.036.854.775.808 to 9.223.372.036.854.775.807 (8 bytes)
					case TC_UINT64:			return "BIGINT";	//	no native support 
					case TC_FLOAT:			return "REAL";		//	float -3.40E + 38 to 3.40E + 38 (4 bytes)
					case TC_DOUBLE:			return "FLOAT(24)";	//	(3 bytes)
					case TC_FLOAT80:		return "FLOAT(53)";	//	
					case TC_STRING:
						{
							BOOST_ASSERT_MSG(width != 0, "column width is null");
							std::ostringstream ostream;
							ostream << "VARCHAR(" << width << ")";
							return ostream.str();
						}
						
					case TC_TIME_POINT:		return "DATETIME";	//	//	'1000-01-01 00:00:00' to '9999-12-31 23:59:59' ('YYYY-MM-DD HH:MM:SS')
					case TC_NANO_SECOND:	return "BIGINT";	
					case TC_MICRO_SECOND:	return "BIGINT";	
					case TC_MILLI_SECOND:	return "BIGINT";	
					case TC_SECOND:			return "BIGINT";	
					case TC_MINUTE:			return "BIGINT";	
					case TC_HOUR:			return "BIGINT";	
// 					case TC_DAYS:			return "BIGINT";	
					case TC_VERSION:		return "INT";
					case TC_REVISION:		return "INT";
					case TC_CODE:			return "INT";
					case TC_BUFFER:			return "BLOB";
// 					case CYY_INDEX:			return "BIGINT UNSIGNED";
// 					case CYY_DIFF:			return "BIGINT";
	// 				case CYY_RGB_COLOR8:	return "rgb8";
	// 				case CYY_RGB_COLOR16:	return "rgb16";
					case TC_MAC48:			return "CHAR(17)";
					case TC_MAC64:			return "CHAR(19)";

					case TC_ATTR_MAP:		return "VARCHAR(2048)";
					case TC_PARAM_MAP:		return "VARCHAR(2048)";
					case TC_ATTR:			return "VARCHAR(512)";	
					case TC_PARAM:			return "VARCHAR(512)";
					case TC_TUPLE:			return "VARCHAR(2048)";
					case TC_VECTOR:			return "VARCHAR(2048)";
					case TC_SET:			return "VARCHAR(2048)";
	// 				case CYY_LOCKABLE:		return "lock";
	// 				case CYY_TABLE:			return "table";
					
					case TC_UUID:			return "CHAR(36)";
					case TC_FS_PATH:		return "CHAR(1024)";
						default:
							break;
					}
					return "BLOB";
					
				case POSTGRESQL:
					//	see https://www.postgresql.org/docs/9.1/static/datatype.html
					//	subtract the smallest negative number so that you effectively get only positives
					switch (code)
					{
					case TC_BOOL:			return "bool";	//	Logical boolean data type - true or false
					case TC_CHAR:
					case TC_INT8:			return "smallint";	//	2-byte signed integer.
					case TC_UINT8:			return "smallint";	//	PostgreSQL doesn't support unsigned types
					case TC_INT16:			return "smallint";	//	CHECK (v > 0)
					case TC_UINT16:			return "smallint";
					case TC_INT32:			return "integer";	//	4-byte
					case TC_UINT32:			return "integer";
					case TC_INT64:			return "bigint";	//	8-byte
					case TC_UINT64:			return "bigint";
					case TC_FLOAT:			return "real";		//	4-byte,
					case TC_DOUBLE:			return "double precision";	//	8-byte
					case TC_FLOAT80:		return "double precision";
					case TC_STRING:
					{
						BOOST_ASSERT_MSG(width != 0, "column width is null");
						std::ostringstream ostream;
						ostream << "VARCHAR(" << width << ")";
						return ostream.str();
					}
					case TC_TIME_POINT:		return "timestamp";	//	'YYYY-MM-DD HH:MM:SS'.
					case TC_NANO_SECOND:	return "bigint";	
					case TC_MICRO_SECOND:	return "bigint";	
					case TC_MILLI_SECOND:	return "bigint";	
					case TC_SECOND:			return "bigint";	
					case TC_MINUTE:			return "bigint";	
					case TC_HOUR:			return "bigint";	
// 					case TC_DAYS:			return "bigint";	
					case TC_VERSION:		return "bigint";
					case TC_REVISION:		return "bigint";
					case TC_CODE:			return "integer";
					case TC_BUFFER:
						{
							BOOST_ASSERT_MSG(width != 0, "column width is null");
							std::ostringstream ostream;
							ostream << "bit varying(" << width << ")";
							return ostream.str();
						}
						
// 					case CYY_INDEX:			return "BIGINT UNSIGNED";
// 					case CYY_DIFF:			return "BIGINT";
	// 				case CYY_RGB_COLOR8:	return "rgb8";
	// 				case CYY_RGB_COLOR16:	return "rgb16";
					case TC_MAC48:			return "macaddr";	// !
					case TC_MAC64:			return "macaddr";

					case TC_ATTR_MAP:		return "text";
					case TC_PARAM_MAP:		return "text";
					case TC_ATTR:			return "text";	
					case TC_PARAM:			return "text";
					case TC_TUPLE:			return "text";
					case TC_VECTOR:			return "text";
					case TC_SET:			return "text";
	// 				case CYY_LOCKABLE:		return "lock";
	// 				case CYY_TABLE:			return "table";
					
					case TC_UUID:			return "uuid";
					case TC_FS_PATH:		return "varchar(1024)";
	//				case CYY_BOOST_ERROR:	return "ec";
// 					case CYY_BOOST_IP_ADDRESS:		
// 						BOOST_ASSERT(width == 40 || width == -1);
// 						return "CHAR(40)";	//	!<	IPv4 and IPv6 as string
// 					case CYY_BOOST_TCP_ENDPOINT:	
// 						BOOST_ASSERT(width == 48 || width == -1);
// 						return "CHAR(48)";	//	address and port as string e.g. [ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff]:65000
	//				case CYY_BOOST_TRIBOOL:	return "tribool";
	//				case CYY_ARRAY:			return "[]";
						default:
							break;
					}
					return "BLOB";
					
					
				case GENERIC:
				default:
					break;
			}
			
			//
			//	generic SQL
			//
			switch (code)
			{
			case TC_BOOL:			return "SMALLINT";	//!<	BOOL is a synonym
			case TC_CHAR:
			case TC_INT8:			return "SMALLINT";	
			case TC_UINT8:			return "INTEGER UNSIGNED";	
			case TC_INT16:			return "INTEGER";	
			case TC_UINT16:			return "INTEGER UNSIGNED";	
			case TC_INT32:			return "INTEGER";	
			case TC_UINT32:			return "INTEGER UNSIGNED";	
			case TC_INT64:			return "INTEGER";	
			case TC_UINT64:			return "BIGINT UNSIGNED";	
			case TC_FLOAT:			return "FLOAT";	
			case TC_DOUBLE:			return "REAL";	
			case TC_FLOAT80:		return "DOUBLE";	
			case TC_STRING:
			{
				BOOST_ASSERT_MSG(width != 0, "column width is null");
				std::ostringstream ostream;
				ostream << "CHARACTER(" << width << ")";
				return ostream.str();
			}
				
			case TC_TIME_POINT:		return "DATETIME";
			case TC_NANO_SECOND:	return "INTEGER";	//!< negative values possible
			case TC_MICRO_SECOND:	return "INTEGER";	
			case TC_MILLI_SECOND:	return "INTEGER";	
			case TC_SECOND:			return "INTEGER";	
			case TC_MINUTE:			return "INTEGER";	
			case TC_HOUR:			return "INTEGER";	
// 					case TC_DAYS:			return "INTEGER";	
			case TC_VERSION:		return "INTEGER UNSIGNED";
			case TC_REVISION:		return "BIGINT UNSIGNED";
			case TC_CODE:			return "INTEGER UNSIGNED";
			case TC_BUFFER:			return "BIT VARYING";
// 					case CYY_INDEX:			return "BIGINT UNSIGNED";
// 					case CYY_DIFF:			return "BIGINT";
// 				case CYY_RGB_COLOR8:	return "rgb8";
// 				case CYY_RGB_COLOR16:	return "rgb16";
			case TC_MAC48:			return "CHARACTER(17)";
			case TC_MAC64:			return "CHARACTER(19)";

			case TC_ATTR_MAP:		return "VARCHAR(2048)";
			case TC_PARAM_MAP:		return "VARCHAR(2048)";
			case TC_ATTR:			return "VARCHAR(512)";	
			case TC_PARAM:			return "VARCHAR(512)";
			case TC_TUPLE:			return "VARCHAR(2048)";
			case TC_VECTOR:			return "VARCHAR(2048)";
			case TC_SET:			return "VARCHAR(2048)";
// 				case CYY_LOCKABLE:		return "lock";
// 				case CYY_TABLE:			return "table";
			
			case TC_UUID:			return "CHARACTER(36)";
			case TC_FS_PATH:		return "VARCHAR(1024)";
//				case CYY_BOOST_ERROR:	return "ec";
// 					case CYY_BOOST_IP_ADDRESS:		
// 						BOOST_ASSERT(width == 40 || width == -1);
// 						return "CHAR(40)";	//	!<	IPv4 and IPv6 as string
// 					case CYY_BOOST_TCP_ENDPOINT:	
// 						BOOST_ASSERT(width == 48 || width == -1);
// 						return "CHAR(48)";	//	address and port as string e.g. [ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff]:65000
//				case CYY_BOOST_TRIBOOL:	return "tribool";
//				case CYY_ARRAY:			return "[]";
				default:
					break;
			}
			return "BLOB";
		}
	}	
}





