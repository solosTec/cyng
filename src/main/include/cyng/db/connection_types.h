/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Sylko Olzscher
 *
 */

#ifndef CYNG_DB_CONNECTION_TYPES_H
#define CYNG_DB_CONNECTION_TYPES_H

#include <string>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/config.hpp>

namespace cyng
{
	namespace db 
	{
		/*	
		*	Define a list of available database connection types
		*/
		enum connection_type 
		{
			CONNECTION_DEFAULT = 0,	//!< fallback if all other definitions are missing
			
			CONNECTION_MYSQL = 1,	//!< requires MySQL Connector C++
			CONNECTION_SQLITE = 2,
			CONNECTION_ODBC = 3,	//!< on linux unixODBC
			CONNECTION_OLEDB = 4,	//!< windows only
		};
		
		/**
		 * @return true if the specified connection type
		 * is available
		 */
		//BOOST_CONSTEXPR //	support from vc is still broken
		inline
		bool is_available(connection_type type) BOOST_NOEXCEPT
		{
			if (type == CONNECTION_MYSQL)
			{
#ifdef CYNG_MYSQL_INSTALLED		
				return true;
#else
				return false;
#endif				
			}


			else if (type == CONNECTION_ODBC)
			{
#ifdef CYNGE_ODBC_INSTALLED			
				return true;
#else
				return false;
#endif
			}

			else if (type == CONNECTION_SQLITE)
			{
#ifdef CYNG_SQLITE3_INSTALLED		
				return true;
#else
				return false;
#endif				
			}

			else if (type == CONNECTION_OLEDB)
			{
#ifdef CYNG_OLEDB_INSTALLED		
				return true;
#else
				return false;
#endif				
			}

			return false;
		}
		
		/**
		 * Implements a convention about connection names.
		 * 
		 * @return enum value of the specified connection name
		 */
		inline connection_type get_connection_type(std::string const& name) BOOST_NOEXCEPT
		{
			if (boost::algorithm::iequals(name, "MySQL"))
			{
				return CONNECTION_MYSQL;
			}

			if (boost::algorithm::iequals(name, "ODBC"))
			{
				return CONNECTION_ODBC;
			}

			if (boost::algorithm::iequals(name, "SQLite"))
			{
				return CONNECTION_SQLITE;
			}

			if (boost::algorithm::iequals(name, "OLEDB"))
			{
				return CONNECTION_OLEDB;
			}

			return CONNECTION_DEFAULT;
		}
	
		
	}
}

#endif	//	CYNG_DB_CONNECTION_TYPES_H
