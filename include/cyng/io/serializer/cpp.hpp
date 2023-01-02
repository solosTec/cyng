/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_IO_CPP_HPP
#define CYNG_IO_CPP_HPP

#include <cyng/io/io.h>
#include <cyng/obj/intrinsics.h>
#include <cyng/obj/tag.hpp>

namespace cyng {
    namespace io {

        /**
         * fallback
         */
        template <typename T> struct serializer<T, CPP> {
            static std::size_t write(std::ostream &os, T const &v) {
                calc_size const cs(os);
                os << '"' << v << '"';
                return cs;
            }
        };

        template <> struct serializer<null, CPP> {
            static std::size_t write(std::ostream &os, null n);
        };

        template <> struct serializer<eod, CPP> {
            static std::size_t write(std::ostream &os, eod e);
        };

        template <> struct serializer<bool, CPP> {
            static std::size_t write(std::ostream &os, bool v);
        };

        template <> struct serializer<std::int8_t, CPP> {
            static std::size_t write(std::ostream &os, std::int8_t v);
        };

        template <> struct serializer<std::int16_t, CPP> {
            static std::size_t write(std::ostream &os, std::int16_t v);
        };

        template <> struct serializer<std::int32_t, CPP> {
            static std::size_t write(std::ostream &os, std::int32_t v);
        };

        template <> struct serializer<std::int64_t, CPP> {
            static std::size_t write(std::ostream &os, std::int64_t v);
        };

        template <> struct serializer<std::uint8_t, CPP> {
            static std::size_t write(std::ostream &os, std::uint8_t v);
        };

        template <> struct serializer<std::uint16_t, CPP> {
            static std::size_t write(std::ostream &os, std::uint16_t v);
        };

        template <> struct serializer<std::uint32_t, CPP> {
            static std::size_t write(std::ostream &os, std::uint32_t v);
        };

        template <> struct serializer<std::uint64_t, CPP> {
            static std::size_t write(std::ostream &os, std::uint64_t v);
        };

        template <> struct serializer<double, CPP> {
            static std::size_t write(std::ostream &os, double v);
        };

        template <typename CharT, typename Traits, typename Allocator>
        struct serializer<std::basic_string<CharT, Traits, Allocator>, CPP> {
            using string_t = std::basic_string<CharT, Traits, Allocator>;
            static std::size_t write(std::ostream &os, string_t const &s) {
                static const type<string_t> type;

                calc_size const cs(os);
                os << '"';
                serializer<string_t, PLAIN>::write(os, s);
                os << '"';

                return cs;
            }
        };

        template <> struct serializer<std::filesystem::path, CPP> {
            static std::size_t write(std::ostream &os, std::filesystem::path const &);
        };

        template <> struct serializer<severity, CPP> {
            static std::size_t write(std::ostream &os, severity s);
        };

        template <> struct serializer<boost::uuids::uuid, CPP> {
            static std::size_t write(std::ostream &os, boost::uuids::uuid const &v);
        };

        template <> struct serializer<vector_t, CPP> {
            static std::size_t write(std::ostream &o, vector_t const &v);
        };

        template <> struct serializer<tuple_t, CPP> {
            static std::size_t write(std::ostream &o, tuple_t const &v);
        };

        template <> struct serializer<param_t, CPP> {
            static std::size_t write(std::ostream &o, param_t const &v);
        };

    } // namespace io
} // namespace cyng
#endif
