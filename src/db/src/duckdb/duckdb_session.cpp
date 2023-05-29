// #include <duckdb/duckdb_connection.h>
// #include <duckdb/duckdb_error.hpp>
#include <duckdb.hpp>
#include <duckdb/duckdb_session.h>
// #include <duckdb/duckdb_statement.h>

#include <cyng/io/serialize.h>
#include <cyng/obj/algorithm/find.h>
#include <cyng/parse/duration.h>
#include <cyng/sql/dialect.h>

#ifdef _DEBUG_DB
#include <iostream>
#endif

namespace cyng {
    namespace db {
        namespace duckdb {
            session::session(int busy_timeout)
                //                : connection_(new connection())
                : busy_timeout_(busy_timeout) {}

            session::~session() { close(); }

            std::pair<std::string, bool> session::connect(param_map_t const &config) { return std::make_pair("", false); }

            bool session::close() {
                // release_memory();
                // return connection_->close();
                return false;
            }

            bool session::is_alive() const { return false; }

            bool session::begin() { return false; }

            bool session::commit() { return false; }

            bool session::rollback() { return false; }

            bool session::is_transaction() const { return false; }

            boost::system::error_code session::last_error() const { return boost::system::error_code(); }

            statement_ptr session::create_statement() { return statement_ptr(); }

            bool session::execute(std::string const &sql) { return true; }

            cyng::sql::dialect session::get_dialect() const { return cyng::sql::dialect::DUCKDB; }

        } // namespace duckdb
    }     // namespace db
} // namespace cyng
