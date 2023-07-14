/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 Sylko Olzscher
 *
 */
#ifndef CYNG_ASM_CODE_H
#define CYNG_ASM_CODE_H

#include <cstdint>
#include <limits>

namespace cyng {
    namespace assembler {
        /**
         * an UTF-8 code point
         */
        class code {
          public:
            constexpr code()
                : c_(std::numeric_limits<std::uint32_t>::max()) {}

            constexpr code(std::uint32_t c)
                : c_(c) {}

            constexpr code(code const &c)
                : c_(c.c_) {}

            constexpr bool is_eof() const { return std::numeric_limits<std::uint32_t>::max() == c_; }

            constexpr bool is_nl() const { return '\n' == c_; }
            constexpr bool is_ws() const {
                return '\t' == c_    // horizontal tab (9)
                       || '\n' == c_ // new line (10)
                       || '\v' == c_ // vertical tab (11)
                       || '\f' == c_ // form feed (12)
                       || '\r' == c_ // carriage return (13)
                       || ' ' == c_  // space (32)
                    ;
            }

            //  static_cast<std::uint32_t>(...)
            constexpr operator std::uint32_t() const { return c_; }

          private:
            std::uint32_t const c_;
        };

        /**
         * Generate an EOF code point
         */
        constexpr code make_eof() { return std::numeric_limits<std::uint32_t>::max(); }

        /**
         * Generate a NL code point
         */
        constexpr code make_nl() { return '\n'; }

        /**
         * Generate a code point
         */
        constexpr code make_code(std::uint32_t c) { return c; }

        constexpr bool is_number(std::uint32_t c) {
            switch (c) {
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0': return true;
            default: break;
            }
            return false;
        }
        constexpr bool is_lower_case_latin(std::uint32_t c) {
            switch (c) {
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z': return true;
            default: break;
            }
            return false;
        }
        constexpr bool is_upper_case_latin(std::uint32_t c) {
            switch (c) {
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z': return true;
            default: break;
            }
            return false;
        }
        constexpr bool is_alpha(std::uint32_t c) { return (is_lower_case_latin(c) || is_upper_case_latin(c)); }
    } // namespace assembler
} // namespace cyng
#endif
