/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2032 Sylko Olzscher
 *
 */

#ifndef CYNG_DB_DUCK_RESULT_H
#define CYNG_DB_DUCK_RESULT_H

#include <cyng.h>

#if defined(CYNG_SQLITE3_CONNECTOR)

#include <cyng/db/details/result_interface.h>
#include <duckdb/duckdb_statement.h>

namespace cyng {
    namespace db {
        namespace duckdb {
            class duckdb_result : public result_interface, public std::enable_shared_from_this<duckdb_result> {
              public:
                duckdb_result(statement::shared_type);

                static result_ptr factory(statement::shared_type);

                virtual object get(int, std::size_t, std::size_t /*= SQL_NTS*/) override;
                virtual std::size_t column_count() override;

              private:
                statement::shared_type statement_;
            };
        } // namespace duckdb
    }     // namespace db
} // namespace cyng

#endif
#endif
