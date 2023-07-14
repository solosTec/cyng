/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 Sylko Olzscher
 *
 */
#ifndef CYNG_ASM_TOKENIZER_H
#define CYNG_ASM_TOKENIZER_H

#include <cyng/asm/code.h>
#include <cyng/asm/token.h>

#include <cstdint>
#include <functional>
#include <string>

namespace cyng {
    namespace assembler {
        class tokenizer {
          private:
            enum class state : std::uint32_t {
                FAILURE,   //  error state
                START,     //  initial state
                COMMENT,   // starts with ';'
                LITERAL,   // anything else
                NUMBER,    // integer
                STRING,    // "..."
                VALUE,     // '...'
                ESCAPE,    // string
                SECTION,   //  .code
                TIMEPOINT, // @2021-08-09T17:12:13
                EOD,       // end of data
            } state_;

          public:
            tokenizer(emit_token_f);
            /**
             * @brief insert next code point
             *
             * @param c
             * @return true to advance
             */
            [[nodiscard]] bool put(code &&c);

          private:
            std::pair<state, bool> start(code const &c);
            std::pair<state, bool> comment(code const &c);
            std::pair<state, bool> section(code const &c);
            std::pair<state, bool> string(code const &c);
            std::pair<state, bool> value(code const &c);
            std::pair<state, bool> escape(code const &c);
            std::pair<state, bool> number(code const &c);
            std::pair<state, bool> literal(code const &c);
            std::pair<state, bool> timepoint(code const &c);

          private:
            std::u32string token_;
            emit_token_f emit;
        };
    } // namespace assembler
} // namespace cyng
#endif //   DOCC_SCRIPT_TOKENIZER_H
