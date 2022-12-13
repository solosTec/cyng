/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_SQL_CONVERT_HPP
#define CYNG_SQL_CONVERT_HPP

#include <cyng/io/ostream.h>
#include <cyng/sql/dialect.h>

namespace cyng {
    namespace sql {
        namespace {
            template <typename T> struct sql_policy {
                static std::string to_sql(dialect d, T v) { return to_string(v); }
            };

            template <> struct sql_policy<std::string> {
                static std::string to_sql(dialect d, std::string const &v) { return "\"" + to_string(v) + "\""; }
            };

            template <std::size_t N> struct sql_policy<const char (&)[N]> {
                static std::string to_sql(dialect d, char const (&p)[N]) { return "'" + to_string(p) + "'"; }
            };

            template <> struct sql_policy<char const *> {

                //	full specialized
                static std::string to_sql(dialect d, char const *p) { return "'" + to_string(p) + "'"; }
            };

            template <> struct sql_policy<std::chrono::system_clock::time_point> {

                //	full specialized
                static std::string to_sql(dialect d, std::chrono::system_clock::time_point const &tp) {
                    return "datetime(" + to_string(tp) + ")";
                }
            };

            template <> struct sql_policy<date> {

                //	full specialized
                static std::string to_sql(dialect d, date const &tp) { return "datetime(" + as_string(tp, "%F %T") + ")"; }
            };
        } // namespace
        template <typename T> std::string to_sql(dialect d, T v) {
            using value_t = std::decay_t<T>;
            return sql_policy<value_t>::to_sql(d, v);
        }

    } // namespace sql
} // namespace cyng

#endif
