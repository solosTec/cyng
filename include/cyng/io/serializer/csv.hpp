/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_IO_CSV_HPP
#define CYNG_IO_CSV_HPP

#include <cyng/io/io.h>

#include <cyng/io/serializer/plain.hpp>
#include <cyng/obj/intrinsics/buffer.h>
#include <cyng/obj/intrinsics/container.h>
#include <cyng/obj/intrinsics/null.h>
#include <cyng/obj/intrinsics/severity.h>
#include <cyng/obj/object.h>

namespace cyng {
    namespace io {

        template <> struct serializer<null, CSV> {
            static std::size_t write(std::ostream &os, null);
        };

        template <> struct serializer<bool, CSV> {
            static std::size_t write(std::ostream &os, bool);
        };

        template <> struct serializer<vector_t, CSV> {
            static std::size_t write(std::ostream &os, vector_t const &);
        };
        template <> struct serializer<tuple_t, CSV> {
            static std::size_t write(std::ostream &os, tuple_t const &);
        };

        template <typename CharT, typename Traits, typename Allocator>
        struct serializer<std::basic_string<CharT, Traits, Allocator>, CSV> {
            using string_t = std::basic_string<CharT, Traits, Allocator>;
            static std::size_t write(std::ostream &os, string_t const &s) {

                calc_size const cs(os);
                os << '"';
                serializer<string_t, PLAIN>::write(os, s);
                os << '"';

                return cs;
            }
        };

    } // namespace io
} // namespace cyng
#endif
