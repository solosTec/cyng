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
			if (boost::algorithm::equals(name, "MySQL"))	return connection_type::MYSQL;
			if (boost::algorithm::equals(name, "ODBC"))	return connection_type::ODBC;
			if (boost::algorithm::equals(name, "SQLite"))	return connection_type::SQLITE;
			if (boost::algorithm::equals(name, "Postgres"))	return connection_type::PQXX;
			return connection_type::NONE;
		}
	}
}

