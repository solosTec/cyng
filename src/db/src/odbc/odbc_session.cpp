/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Sylko Olzscher
 *
 */

#include <odbc/odbc_diag.hpp>
#include <odbc/odbc_session.h>
#include <odbc/odbc_statement.h>

#include <cyng/obj/algorithm/find.h>

namespace cyng {
    namespace db {
        namespace odbc {

            session::session(SQLUINTEGER login_timeout, SQLUINTEGER connect_timeout)
                : connection_()
                , login_timeout_(login_timeout)
                , connect_timeout_(connect_timeout)
                , sql_dialect_(cyng::sql::dialect::GENERIC) {}

            session::~session() { close(); }

            std::pair<std::string, bool> session::connect(param_map_t const &config) {

                std::string connect =
                    cyng::find_value(config, std::string("connect"), std::string("DSN=db;Uid=uid;Pwd=pwd;AUTO_RECONNECT=0;"));
                auto const readonly = find_value(config, std::string("readonly"), false);
                if (readonly && (connect.find("READONLY") == std::string::npos)) {
                    connect.append("READONLY=1;");
                }
                SQLUINTEGER const login_timeout = cyng::find_value(config, std::string("login.timeout"), login_timeout_);
                SQLUINTEGER const connect_timeout = cyng::find_value(config, std::string("connect.timeout"), connect_timeout_);
                std::string const dialect_name = cyng::find_value(config, std::string("dialect"), std::string("MYSQL"));
                sql_dialect_ = cyng::sql::get_dialect(dialect_name);

                {
                    //	Login timeout is to set before connection
                    const SQLRETURN rc =
                        ::SQLSetConnectAttr(connection_, SQL_ATTR_LOGIN_TIMEOUT, reinterpret_cast<SQLPOINTER>(login_timeout), 0);

                    if (!is_ok(rc)) {
                        connection_diagnostics dia;
                        dia.run(connection_);
                        return std::make_pair(std::string(), false);
                    }
                }

                {
                    //	connection timeout
                    const SQLRETURN rc = ::SQLSetConnectAttr(
                        connection_, SQL_ATTR_CONNECTION_TIMEOUT, reinterpret_cast<SQLPOINTER>(connect_timeout), 0);
                    if (!is_ok(rc)) {
                        connection_diagnostics dia;
                        dia.run(connection_);
                        return std::make_pair(std::string(), false);
                    }
                }

                SQLCHAR out_str[1024] = {0};
                SQLSMALLINT str_length = 0; //	string length of out_str
                {

                    const SQLRETURN rc = ::SQLDriverConnect(
                        connection_ // hdbc_
                        ,
                        NULL //	window handle
                        ,
                        (SQLCHAR *)connect.c_str(),
                        connect.length() //	or SQL_NTS
                        ,
                        out_str,
                        static_cast<SQLSMALLINT>(sizeof(out_str)),
                        &str_length,
                        SQL_DRIVER_NOPROMPT);

                    if (!is_ok(rc)) {
                        connection_diagnostics dia;
                        dia.run(connection_);
                        return std::make_pair(std::string(), false);
                    } else {
                        //	set NULL termination
                        BOOST_ASSERT_MSG(str_length < 1024, "str_length out of range");
                        out_str[str_length] = '\0';
                    }
                }

                return std::make_pair(std::string((const char *)&out_str[0], str_length), true);
            }

            bool session::close() {
                if (!is_alive()) {
                    commit();
                    const SQLRETURN rc = ::SQLDisconnect(connection_);
                    return is_ok(rc);
                }
                return false;
            }

            bool session::is_alive() const {
                if (connection_.is_valid()) {

                    //
                    //	This method doesn't work as expected in all cases.
                    //	Avoid ODBC connection pooling. This gives the false impression
                    //	that an closed connection is already open.
                    //

                    SQLUINTEGER status = SQL_CD_TRUE; //	Connection is closed/dead
                    SQLRETURN rc = ::SQLGetConnectAttr(
                        connection_, SQL_ATTR_CONNECTION_DEAD, (SQLPOINTER)&status, (SQLINTEGER)sizeof(status), NULL);

                    return ((rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO) && (status == SQL_CD_FALSE));
                }
                return false;
            }

            bool session::begin() {
                const SQLRETURN rc =
                    ::SQLSetConnectAttr(connection_, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, SQL_IS_UINTEGER);

                if (!is_ok(rc)) {
                    connection_diagnostics dia;
                    dia.run(connection_);
                    return false;
                }
                return true;
            }

            bool session::commit() {
                const SQLRETURN rc = ::SQLEndTran(SQL_HANDLE_DBC, connection_, SQL_COMMIT);

                if (!is_ok(rc)) {
                    connection_diagnostics dia;
                    dia.run(connection_);
                    return false;
                }

                //	go back to autocommit mode
                return is_ok(::SQLSetConnectAttr(connection_, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_IS_UINTEGER));
            }

            bool session::rollback() {
                const SQLRETURN rc = ::SQLEndTran(SQL_HANDLE_DBC, connection_, SQL_ROLLBACK);

                if (!is_ok(rc)) {
                    connection_diagnostics dia;
                    dia.run(connection_);
                    return false;
                }

                //	goto autocommit mode
                return is_ok(::SQLSetConnectAttr(connection_, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_IS_UINTEGER));
            }

            bool session::is_transaction() const {
                SQLINTEGER ac = 0;

                if (!is_ok(::SQLGetConnectAttr(connection_, SQL_ATTR_AUTOCOMMIT, &ac, 0, 0))) {
                    return false;
                }

                return (SQL_AUTOCOMMIT_ON == ac);
            }

            boost::system::error_code session::last_error() const {
                connection_diagnostics dia;
                dia.run(connection_);

                return (dia.empty()) ? boost::system::error_code() : error::make_error_code(dia.get_messages().front().get_code());
            }

            statement_ptr session::create_statement() { return statement_factory(connection_); }

            bool session::execute(std::string const &sql) { return create_statement()->execute(sql); }

            cyng::sql::dialect session::get_dialect() const { return sql_dialect_; }

        } // namespace odbc
    }     // namespace db
} // namespace cyng
