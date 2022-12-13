/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */

#ifndef CYNG_SQL_DSL_VARIABLE_HPP
#define CYNG_SQL_DSL_VARIABLE_HPP

#include <cyng/sql/dialect.h>
#include <cyng/store/meta.h>

#include <chrono>
#include <ostream>

namespace cyng {
    namespace sql {
        template <typename T> struct variable {
            variable(T &v)
                : value_(v) {}

            std::string to_string(dialect d, meta_sql const &) const {
                std::stringstream ss;
                ss << value_;
                return ss.str();
            }

            T &value_;
        };

        /**
         * std::string
         */
        template <> struct variable<std::string> {
            variable(std::string &v);
            std::string to_string(dialect d, meta_sql const &) const;

            std::string &value_;
        };

        /**
         * boolean
         */
        template <> struct variable<bool> {
            variable(bool &b);
            std::string to_string(dialect d, meta_sql const &) const;

            bool &b_;
        };

        /**
         * timepoint
         */
        template <> struct variable<std::chrono::system_clock::time_point> {
            variable(std::chrono::system_clock::time_point &tp);
            std::string to_string(dialect d, meta_sql const &) const;

            std::chrono::system_clock::time_point &tp_;
        };

        /**
         * date
         */
        template <> struct variable<date> {
            variable(date &tp);
            std::string to_string(dialect d, meta_sql const &) const;

            date &tp_;
        };

        /**
         * factory - only references allowed
         */
        template <typename T> inline variable<T> make_variable(T &ref) { return variable<T>(ref); }
    } // namespace sql
} // namespace cyng

#endif //	CYNG_SQL_DSL_VARIABLE_HPP
