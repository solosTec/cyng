/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 Sylko Olzscher
 *
 */
#ifndef CYNG_ASM_PARSER_H
#define CYNG_ASM_PARSER_H

#include <token.h>

#include <cyng/obj/object.h>

#include <cstdint>
#include <functional>
#include <string>

namespace cyng {
    namespace assembler {
        /**
         * Parser for assembler code.
         * This approach is intriguingly simple. Each expression consists of an assembler
         * instruction and an optional value.
         * The data section is unused yet. Maybe in the future this can be used with a more
         * complex syntax to simplify using constant values and intrinscis.
         */
        class parser {
          public:
            using cb_f = std::function<void(object)>;

          private:
            class code {
              private:
                enum class state { FAILURE, OP, ARG } state_;

              public:
                code(cb_f);
                void put(token &&c);

              private:
                state op(token &&c);
                state arg(token &&c);

              private:
                cb_f emit;
            };
            class data {
              public:
                data();
                void put(token &&c);
            };

          private:
            enum class mode { DATA, CODE, UNDEF } mode_;

          public:
            parser(cb_f);
            /**
             * @brief insert next code point
             *
             * @param c
             */
            void put(token &&c);

          private:
            code code_;
            data data_;
        };
    } // namespace assembler
} // namespace cyng
#endif
