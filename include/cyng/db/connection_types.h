/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#ifndef CYNG_DB_CONNECTION_TYPES_H
#define CYNG_DB_CONNECTION_TYPES_H

#include <cyng.h>
#include <string>

namespace cyng {
    namespace db {
        /*
         *	Define a list of available database connection types
         */
        enum class connection_type {
            NONE = 0, //!< fallback if all other definitions are missing

            MYSQL = 1, //!< requires MySQL Connector C++
            ODBC = 2,  //!< on linux unixODBC
            PQXX = 3,  //!< PostgreSQL
            SQLITE = 4,
            DUCKDB = 5 //!< Duck DB (embedded)
        };

        /**
         * @return true if the specified connection type
         * is available
         */
        constexpr bool is_installed(connection_type type) {

#if defined(CYNG_MYSQL_CONNECTOR)
            if (type == connection_type::MYSQL)
                return true;
#endif

#if defined(CYNG_ODBC_CONNECTOR)
            if (type == connection_type::ODBC)
                return true;
#endif

#if defined(CYNG_PQXX_CONNECTOR)
            if (type == PQXX)
                return true;
#endif

#if defined(CYNG_SQLITE3_CONNECTOR)
            if (type == connection_type::SQLITE)
                return true;
#endif

#if defined(CYNG_DUCKDB_CONNECTOR)
            if (type == connection_type::DUCKDB)
                return true;
#endif

            return false;
        }

        /**
         * convert a string into an connection type. Accepted values are
         * MySQL, ODBC, SQLite and Postgres
         */
        connection_type to_connection_type(std::string const &name) noexcept;
    } // namespace db
} // namespace cyng

#endif
