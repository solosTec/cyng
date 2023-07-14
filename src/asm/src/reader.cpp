
#include <cyng/asm/reader.h>

#include <cyng/asm/code.h>
#include <cyng/asm/position.h>

#include <cyng/io/parser/stream.hpp>
#include <cyng/io/serialize.h>
#include <cyng/log/record.h>

#include <fstream>
#include <iostream>

// #include <boost/container_hash/hash.hpp>

namespace cyng {
    namespace assembler {
        reader::reader(logger _)
            : logger_(_)
            , parser_([this](object obj) -> void {
                //
                CYNG_LOG_TRACE(logger_, "(" << obj << ":" << obj.rtti().type_name() << ")");
            })
            , tokenizer_([this](token &&tok) -> void {
                //  forward to parser
                CYNG_LOG_TRACE(logger_, tok);
                parser_.put(std::move(tok));
            }) {}

        std::uint64_t reader::start(std::filesystem::path p) {
            std::uint64_t line{1};
            std::ifstream stream(p.string()); // read-only
            if (stream.is_open()) {

                //  don't skip white spaces
                stream.unsetf(std::ios::skipws);

                auto [a, b] = get_stream_range(stream);
                BOOST_ASSERT(a != b);

                //
                //  read optional BOM
                //
                auto [pos, end] = utf8::get_utf8_range(a, b);

                //
                // read file
                //
                std::uint32_t column{1};
                std::size_t seed = 0;
                for (; pos != end; ++pos) {
                    //
                    // send to tokenizer
                    //
                    // boost::hash_combine(seed, *pos);

                    ++column;
                    if (*pos == '\n') {
                        ++line;
                        column = 0;
                        CYNG_LOG_TRACE(logger_, "line " << line);
                        while (!tokenizer_.put(make_nl())) {
                            ; // retry
                        }

                    } else {
                        while (!tokenizer_.put(make_code(*pos))) {
                            ; // retry
                        }
                    }
                }

                //
                //  EOF
                //
                while (!tokenizer_.put(make_eof())) {
                    ; // retry
                }

            } else {
                CYNG_LOG_ERROR(logger_, "file " << p << "not found");
            }
            return line;
        }
    } // namespace assembler
} // namespace cyng
