#include <cyng/asm/tokenizer.h>

#include <cyng/io/parser/stream.hpp>
// #include <cyng/parse/op.h>

#ifdef _DEBUG
#include <iostream>
#endif

#include <boost/algorithm/string.hpp>
#include <boost/assert.hpp>

namespace cyng {
    namespace assembler {
        tokenizer::tokenizer(emit_token_f cb)
            : state_(state::START)
            , token_{}
            , emit(cb) {
            //  should not be empty
            BOOST_ASSERT(cb);
        }

        bool tokenizer::put(code &&c) {
            //
            bool advance{true};
            switch (state_) {
            case state::START: std::tie(state_, advance) = start(c); break;
            case state::COMMENT: std::tie(state_, advance) = comment(c); break;
            case state::SECTION: std::tie(state_, advance) = section(c); break;
            case state::STRING: std::tie(state_, advance) = string(c); break;
            case state::ESCAPE: std::tie(state_, advance) = escape(c); break;
            case state::VALUE: std::tie(state_, advance) = value(c); break;
            case state::LITERAL: std::tie(state_, advance) = literal(c); break;
            case state::NUMBER: std::tie(state_, advance) = number(c); break;
            case state::TIMEPOINT: std::tie(state_, advance) = timepoint(c); break;
            case state::EOD: break;
            default: break;
            }

            return advance;
        }

        std::pair<tokenizer::state, bool> tokenizer::start(code const &c) {

            //
            //  reset token
            //
            token_.clear();

            //
            //  select next state
            //
            switch (static_cast<std::uint32_t>(c)) {
            case ';': return {state::COMMENT, true};
            case '.': return {state::SECTION, true};
            case '"': return {state::STRING, true};
            case '\'': return {state::VALUE, true};
            case make_eof(): return {state::EOD, true};
            case '(':
            case ')':
            case '[':
            case ']':
            case ',':
            case '*':
            case '+':
            case '/':
            case '-':
            case '#':
            case 182: // ¶ (ISO-8859-1) - std::string("\xc2\xb6")
                //  symbols
                emit({token_type::SYM, std::u32string(1, c)});
                return {state_, true};
            case '@': return {state::TIMEPOINT, true};

            default: break;
            }

            if (c.is_ws()) {
                return {state_, true};
            }
            return {(is_number(c)) ? state::NUMBER : state::LITERAL, false};
        }
        std::pair<tokenizer::state, bool> tokenizer::comment(code const &c) {
            if (c.is_nl()) {
                return {state::START, true};
            }
            return {state_, true};
        }
        std::pair<tokenizer::state, bool> tokenizer::section(code const &c) {
            if (!is_alpha(c)) {
                emit({token_type::SEC, token_});
                return {state::START, false};
            }
            token_ += c;
            return {state_, true};
        }
        std::pair<tokenizer::state, bool> tokenizer::string(code const &c) {
            switch (static_cast<std::uint32_t>(c)) {
            case '"':
                //  complete
                emit({token_type::STR, token_});
                return {state::START, true};
            case '\\': return {state::ESCAPE, true};
            default: break;
            }
            token_ += c;
            return {state_, true};
        }
        std::pair<tokenizer::state, bool> tokenizer::escape(code const &c) {
            switch (static_cast<std::uint32_t>(c)) {
            case '"':
            case '\\':
            case '\n':
            case '\t':
                //  special character
                token_ += c;
                return {state::STRING, true};

            default: break;
            }
            return {state_, false};
        }
        std::pair<tokenizer::state, bool> tokenizer::value(code const &c) {
            if ('\'' == c) {
                //  complete
                emit({token_type::VAL, token_});
                return {state::START, true};
            }
            token_ += c;
            return {state_, true};
        }
        std::pair<tokenizer::state, bool> tokenizer::literal(code const &c) {
            if (c == ':') {
                //  label
                emit({token_type::LAB, token_});
                return {state::START, true};
            }
            if (is_alpha(c) || is_number(c) || c == '_') {
                token_ += c;
                return {state_, true};
            }
            if (!token_.empty()) {
                emit({token_type::LIT, token_});
                return {state::START, false};
            }
            //  skip unknown code point
            return {state::START, true};
        }
        std::pair<tokenizer::state, bool> tokenizer::number(code const &c) {
            if (is_number(c)) {
                token_ += c;
                return {state_, true};
            }
            if (!token_.empty()) {
                emit({token_type::NUM, token_});
                return {state::START, false};
            }
            return {state::START, false};
        }
        std::pair<tokenizer::state, bool> tokenizer::timepoint(code const &c) {
            if (c.is_ws()) {
                //  e.g. 2021-08-09T17:12:13
                BOOST_ASSERT(token_.size() == 19);
                emit({token_type::DAT, token_});
                return {state::START, true};
            }
            token_ += c;
            return {state_, true};
        }

    } // namespace assembler
} // namespace cyng
