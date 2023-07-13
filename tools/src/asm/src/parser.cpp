#include <parser.h>

#include <cyng/obj/factory.hpp>
#include <cyng/parse/op.h>
#include <cyng/parse/string.h>
#include <cyng/parse/timestamp.h>

#ifdef _DEBUG
#include <iostream>
#endif

#include <boost/algorithm/string.hpp>
#include <boost/assert.hpp>

namespace cyng {
    namespace assembler {
        parser::parser(cb_f cb)
            : mode_(mode::UNDEF)
            , code_(cb)
            , data_() {}
        void parser::put(token &&tok) {
            auto const value = tok.get_value();
            //  section
            if (tok == token_type::SEC) {
                if (boost::algorithm::equals("code", value)) {
                    mode_ = mode::CODE;
                } else if (boost::algorithm::equals("data", value)) {
                    mode_ = mode::DATA;
                } else {
                    //  error
                }
            } else {
                switch (mode_) {
                case mode::CODE: return code_.put(std::move(tok));
                case mode::DATA: return data_.put(std::move(tok));
                default: break;
                };
            }
        }

        parser::code::code(cb_f cb)
            : state_(state::OP)
            , emit(cb) {}

        void parser::code::put(token &&tok) {
            switch (state_) {
            case state::OP: state_ = op(std::move(tok)); break;
            case state::ARG: state_ = arg(std::move(tok)); break;
            default: break;
            }
        }

        parser::code::state parser::code::op(token &&tok) {
            //
            auto const oc = to_op(tok.get_value());
            emit(make_object(oc));
            switch (oc) {
            case op::PUSH:
            case op::INVOKE_R: return state::ARG;
            default: break;
            }
            return state_;
        }
        parser::code::state parser::code::arg(token &&tok) {
            // create object
            auto const value = tok.get_value();
            if (tok == token_type::NUM) {
                emit(make_object(to_numeric<std::uint64_t>(value)));
            } else if (tok == token_type::STR) {
                emit(make_object(value));
            } else if (tok == token_type::LIT) {
                emit(make_object(value));
            } else if (tok == token_type::DAT) {
                emit(make_object(to_timestamp(value)));
            } else if (tok == token_type::VAL) {
                //
                // analyse string
                // e.g. uuid:79bf3ba0-2362-4ea5-bcb5-ed93844ac59a
                //
                auto const vec = split(value, ":");
                if (vec.size() == 2) {
                    if (boost::algorithm::equals("uuid", vec.at(0))) {
                        emit(make_object(to_uuid(vec.at(1))));
                    }
                } else {
                    emit(make_object(value));
                }
            }
            return state::OP;
        }

        parser::data::data() {}

        void parser::data::put(token &&c) {}

    } // namespace assembler

} // namespace cyng
