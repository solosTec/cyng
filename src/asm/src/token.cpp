
#include <cyng/asm/token.h>

#include <cyng/io/parser/utf-8.hpp>

namespace cyng {
    namespace assembler {

        std::string token::get_value() const {
            utf8::u32_to_u8_iterator<std::u32string::const_iterator> start(value_.begin());
            utf8::u32_to_u8_iterator<std::u32string::const_iterator> end(value_.end());
            return {start, end};
        }

        /**
         * Streaming operator
         */
        std::ostream &operator<<(std::ostream &os, const token &tok) {
            os << '<' << to_string(tok.type_) << ':' << tok.get_value() << '>';
            return os;
        }

        bool operator==(token const &tok, token_type tt) { return tok.type_ == tt; }

    } // namespace assembler
} // namespace cyng
