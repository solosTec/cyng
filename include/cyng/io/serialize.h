/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_IO_SERIALIZE_H
#define CYNG_IO_SERIALIZE_H

#include <cyng/io/serialize.hpp>
#include <cyng/io/serializer/json_walker.h>
#include <cyng/io/serializer/pretty_walker.h>
#include <cyng/obj/intrinsics/container.h>

#include <ostream>
#include <sstream>

namespace cyng {
    class object;
    namespace io {

        //
        //	serialization tags
        //

        /**
         * Simple serialization to human readable text
         * struct PLAIN {};
         */
        template <typename T> std::size_t serialize_plain(std::ostream &os, T const &value) {
            using serial_t = serializer<T, PLAIN>;
            return serial_t::write(os, value);
        }
        /**
         * Special form for objects required
         */
        std::size_t serialize_plain(std::ostream &, object const &);

        template <typename T> [[nodiscard]] std::string to_plain(T const &value) {
            std::stringstream ss;
            serialize_plain(ss, value);
            return ss.str();
        }

        /**
         * Serialization to readable output
         * with type information
         * struct TYPED {};
         */
        template <typename T> std::size_t serialize_typed(std::ostream &os, T const &value) {
            // return serialize<TYPED>::write(os, value);
            using serial_t = serializer<T, TYPED>;
            return serial_t::write(os, value);
        }

        std::size_t serialize_typed(std::ostream &, object const &);

        template <typename T> [[nodiscard]] std::string to_typed(T const &value) {
            std::stringstream ss;
            serialize_typed(ss, value);
            return ss.str();
        }

        /**
         * print a hierarchical, tree-like structure
         */
        template <typename T> std::ostream &serialize_pretty(std::ostream &os, T const &value) {
            pretty_walker walker(os);
            traverse(value, walker);
            return os;
        }

        template <typename T> std::string [[nodiscard]] to_pretty(T const &value) {
            std::stringstream ss;
            serialize_pretty<T>(ss, value);
            return ss.str();
        }

        /**
         * Serialization to an internal binary
         * format with TLV (type-length-value) encoding.
         * struct BINARY {};
         *
         * @return bytes that were written
         */
        std::size_t serialize_binary(std::ostream &, object const &);

        /**
         * Serialization to JSON.
         * struct JSON {};
         */
        std::size_t serialize_json(std::ostream &, object const &);
        template <typename T> std::size_t serialize_json(std::ostream &os, T const &value) {
            using serial_t = serializer<T, JSON>;
            return serial_t::write(os, value);
        }

        template <typename T> [[nodiscard]] std::string to_json(T const &value) {
            std::stringstream ss;
            serialize_json(ss, value);
            return ss.str();
        }

        /**
         * Serialize JSON with line breaks using the json_walker
         */
        template <typename T> std::ostream &serialize_json_pretty(std::ostream &os, T const &value) {
            json_walker walker(os);
            traverse(value, walker);
            return os;
        }
        template <typename T> [[nodiscard]] std::string to_json_pretty(T const &value) {
            std::stringstream ss;
            serialize_json_pretty<T>(ss, value);
            return ss.str();
        }

        /**
         * Serialization to XML.
         * struct XML {};
         *
         * For correct representation of NULL values the XML namespace
         * xmlns:xsi = "w3.org/2001/XMLSchema-instance"
         * must be declared.
         *
         * @see https://www.w3.org/TR/2004/REC-xmlschema-2-20041028/datatypes.html#built-in-datatypes
         */
        void serialize_xml(std::ostream &, object const &, std::string root_name);
        [[nodiscard]] std::string to_xml(object const &, std::string root_name);
        bool to_xml_file(object const &, std::string file_name, std::string root_name);

        /**
         * Serialization to CSV.
         * struct CSV {};
         */
        std::size_t serialize_csv(std::ostream &, object const &);
        [[nodiscard]] std::string to_csv(object const &);

        /**
         * Serialization to CPP.
         * struct CPP {};
         */
        std::size_t serialize_cpp(std::ostream &, object const &);
        [[nodiscard]] std::string to_cpp(object const &);

    } // namespace io
} // namespace cyng
#endif
