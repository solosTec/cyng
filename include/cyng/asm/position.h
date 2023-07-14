/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 Sylko Olzscher
 *
 */
#ifndef CYNG_ASM_POSITION_H
#define CYNG_ASM_POSITION_H

#include <cstdint>
#include <limits>
#include <string_view>
#include <utility>

namespace cyng {
    namespace assembler {
        class position {
          public:
            constexpr position() = delete;

            explicit constexpr position(std::uint64_t line, std::uint32_t column, std::string_view source)
                : line_(line)
                , column_(column)
                , source_(source) {}

            constexpr position(position const &pos)
                : line_(pos.line_)
                , column_(pos.column_)
                , source_(pos.source_) {}

            //  static_cast<std::pair<std::uint64_t, std::uint32_t>>(...)
            constexpr operator std::pair<std::uint64_t, std::uint32_t>() const { return {line_, column_}; }

            constexpr position &operator=(position &&) = default;

          private:
            std::uint64_t line_;
            std::uint32_t column_;
            std::string_view source_;
        };

        /**
         * Generate an EOF token
         */
        constexpr position make_position(std::uint64_t line, std::uint32_t column, std::string_view source) {
            return position(line, column, source);
        }
    } // namespace assembler
} // namespace cyng
#endif
