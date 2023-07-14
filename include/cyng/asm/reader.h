/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 Sylko Olzscher
 *
 */
#ifndef CYNG_ASM_READER_H
#define CYNG_ASM_READER_H

#include <cyng/log/logger.h>

#include <cyng/asm/parser.h>
#include <cyng/asm/tokenizer.h>

#include <filesystem>

namespace cyng {
    namespace assembler {
        class reader {

          public:
            reader(logger);

            /**
             * start reading
             * @return Number of lines read. A value of null indicates an error.
             */
            std::uint64_t start(std::filesystem::path);

          private:
            logger logger_;
            tokenizer tokenizer_;
            parser parser_;
        };

        /**
         * @return start and end iterator of the specified file
         */
        template <typename CharType>
        std::pair<std::istream_iterator<CharType>, std::istream_iterator<CharType>>
        get_stream_range(std::basic_ifstream<CharType> &fs) {

            return std::make_pair(
                fs.is_open() ? std::istream_iterator<CharType>(fs) : std::istream_iterator<CharType>(),
                std::istream_iterator<CharType>());
        }
    } // namespace assembler
} // namespace cyng

#endif
