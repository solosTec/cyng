/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SQL_DIALECT_H
#define CYNG_SQL_DIALECT_H

#include <string>

namespace cyng 
{
	namespace sql
	{
		/**
		 * List of supported SQL dialects
		 */
		enum dialect 
		{
			GENERIC,	//!<	fallback database type
			MYSQL,		//!<	MySQL Database
			SQLITE,		//!<	SQLite
			ORACLE,		//!<	Oracle DB
			ACCESS,		//!<	MS Access
			SQLSERVER,	//!<	MS SQL Server
			POSTGRESQL,	//!<	PostgreSQL 
		};
		
		/**
		 * @return dialect type 
		 */
		dialect get_dialect(std::string const& name);
		
		/**
		 * List of supported SQL dialects
		 */
		enum feature
		{
			IF_NOT_EXISTS,	//!<	supports create if not exists
			DATE_TIME_SUPPORT,	//!<	offers support for SQL DATE_TIME type
		};

		/**
		 * @return true if SQL dialect support sepcified feature.
		 */
		bool has_feature(dialect, feature);
		
		/**
		 * Maps the C++ data type to an SQL data type
		 * of the specified SQL dialect.
		 */
		std::string get_field_type(dialect d, std::size_t code, std::size_t width = 0);
		
	}	
}

#endif	//	CYNG_SQL_DIALECT_H


