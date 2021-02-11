/*
* The MIT License (MIT)
*
* Copyright (c) 2021 Sylko Olzscher
*
*/


#include <cyng/db/connection_types.h>
#include <boost/algorithm/string.hpp>

namespace cyng
{
	namespace db 
	{
		connection_type to_connection_type(std::string const& name) noexcept {
			if (boost::algorithm::equals(name, "MySQL"))	return connection_type::CONNECTION_MYSQL;
			if (boost::algorithm::equals(name, "ODBC"))	return connection_type::CONNECTION_ODBC;
			if (boost::algorithm::equals(name, "SQLite"))	return connection_type::CONNECTION_SQLITE;
			if (boost::algorithm::equals(name, "Postgres"))	return connection_type::CONNECTION_PQXX;
			return connection_type::CONNECTION_NONE;
		}
	}
}

