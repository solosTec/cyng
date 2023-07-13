/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 Sylko Olzscher
 *
 */
#ifndef CYNG_ASM_TOKEN_H
#define CYNG_ASM_TOKEN_H

#include <cstdint>
#include <functional>
#include <iostream>
#include <limits>
#include <string>
#include <string_view>

namespace cyng {
    namespace assembler {

        enum class token_type : std::uint32_t {
            ERR, // error
            EOD, //	end-of-data
            LIT, //	literal
            NUM, //	integer
            SEC, //	section
            STR, //	string
            VAL, //	value, object
            LAB, //	label
            SYM, // symbols - unused yet
            DAT, // timepoint
        };

        /**
         * textual representation
         */
        constexpr std::string_view to_string(token_type tt) {
            switch (tt) {
            case token_type::ERR: return "ERR";
            case token_type::EOD: return "EOD";
            case token_type::LIT: return "LIT";
            case token_type::NUM: return "NUM";
            case token_type::SEC: return "SEC";
            case token_type::STR: return "STR";
            case token_type::LAB: return "LAB";
            case token_type::VAL: return "VAL";
            case token_type::SYM: return "SYM";
            case token_type::DAT: return "DAT";
            default: break;
            }
            return "";
        }

        class token {
            friend std::ostream &operator<<(std::ostream &os, const token &tok);

          public:
            constexpr token() = delete;
            constexpr token(token &&) = default;

            constexpr token(token_type tt)
                : type_(tt)
                , value_{} {}

            constexpr token(token_type tt, std::u32string value)
                : type_(tt)
                , value_(value) {}

            constexpr token(token const &tok)
                : type_(tok.type_)
                , value_(tok.value_) {}

            /**
             * assignment
             */
            constexpr token &operator=(token &&) = default;

            /**
             * comparison
             */
            friend auto operator<=>(token const &, token const &) = default;

            /**
             * Compare token type only
             */
            friend bool operator==(token const &, token_type);

            std::string get_value() const;

          private:
            token_type type_;
            std::u32string value_;
        }; // namespace cyng

        /**
         * Define an emit function
         */
        using emit_token_f = std::function<void(token &&)>;

    } // namespace assembler
} // namespace cyng
#endif
