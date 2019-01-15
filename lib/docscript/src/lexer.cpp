/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/docscript/lexer.h>
#include <boost/assert.hpp>
#include <boost/algorithm/string.hpp>

namespace cyng	
{
	namespace docscript
	{

		lexer::lexer(emit_symbol_f f, std::function<void(cyng::logging::severity, std::string)> err)
			: state_(STATE_START_)
			, emit_(f)
			, err_(err)
			, tmp_()
			, env_open_()
			, env_close_()
			, env_on_(false)
			, state_stack_()
			, col_(0u)
		{
			state_stack_.push(STATE_ERROR_);
		}

		bool lexer::complete()
		{
			return state_stack_.size() == 1;
		}

		bool lexer::next(std::uint32_t c)
		{
			if (state_stack_.empty()) {
				err_(cyng::logging::severity::LEVEL_ERROR, get_state_name(state_) + ": internal error - empty stack");
			}

			//
			//	updating column
			//
			switch (c) {
			case '\n':
			case U'¶':	//	0xb6
				//	reset virtual line position
				col_ = 0;
			default:
				break;
			}

			bool advance = true;
			switch (state_)
			{
			case STATE_START_:
				std::tie(state_, advance) = state_start(c);
				break;
			case STATE_ERROR_:
				std::tie(state_, advance) = state_error(c);
				break;
			case STATE_SKIP_WS_:
				std::tie(state_, advance) = skip_ws(c);
				break;

			case STATE_NL_DOT_:
			case STATE_WS_DOT_:
				std::tie(state_, advance) = state_dot(c);
				break;

			case STATE_NUMBER_:
				std::tie(state_, advance) = state_number(c);
				break;

			case STATE_TOKEN_:
				std::tie(state_, advance) = state_token(c);
				break;

			case STATE_FUNCTION_:
				std::tie(state_, advance) = state_function(c);
				break;

			case STATE_1_ARG_:
				std::tie(state_, advance) = state_1_arg(c);
				break;

			case STATE_PARAMS_:
				std::tie(state_, advance) = state_params(c);
				break;

			case STATE_VALUE_:
				std::tie(state_, advance) = state_value(c);
				break;

			case STATE_QUOTE_:
				std::tie(state_, advance) = state_quote(c);
				break;

			case STATE_TEXT_:
				std::tie(state_, advance) = state_text(c);
				break;

			case STATE_ENV_OPEN_:
				std::tie(state_, advance) = state_env_open(c);
				break;

			case STATE_ENV_CLOSE_:
				std::tie(state_, advance) = state_env_close(c);
				break;

			case STATE_VERBATIM_:
				std::tie(state_, advance) = state_verbatim(c);
				break;

			case STATE_TRAIL_:
				std::tie(state_, advance) = state_trail(c);
				break;

			default:
				err_(cyng::logging::severity::LEVEL_ERROR, get_state_name(state_) + ": internal error - unknown lexer state");
				state_ = STATE_ERROR_;
				break;
			}

			if (advance)	++col_;
			return advance;
		}


		void lexer::emit(symbol&& s) const
		{
			emit_(std::move(s));
		}

		bool lexer::emit_tmp(symbol_type t)
		{
			if (!tmp_.empty())
			{
				emit(symbol(t, tmp_));
				tmp_.clear();
				return true;
			}
			return false;
		}

		std::string lexer::get_state_name(state s) const
		{
			switch (s)
			{
			case STATE_ERROR_:		return "ERROR";
			case STATE_SKIP_WS_:	return "SKIP_WS";
			case STATE_START_:		return "START";
			case STATE_NL_DOT_:		return "NL>DOT";
			case STATE_WS_DOT_:		return "WS>DOT";
			case STATE_NUMBER_:		return "NUMBER";
			case STATE_TOKEN_:		return "TOKEN";
			case STATE_FUNCTION_:	return "FUNCTION";
			case STATE_1_ARG_:		return "1-ARG";
			case STATE_PARAMS_:		return "PARAMS";
			case STATE_VALUE_:		return "VALUE";
			case STATE_QUOTE_:		return "QUOTE";
			case STATE_TEXT_:		return "TEXT";
			case STATE_ENV_OPEN_:	return "ENV-OPEN";
			case STATE_ENV_CLOSE_:	return "ENV-CLOSE";
			case STATE_VERBATIM_:	return "VERBATIM";
			case STATE_TRAIL_:		return "TRAIL";

			default:
				break;
			}
			return "unknown lexer state";

		}

		std::pair<lexer::state, bool> lexer::state_start(std::uint32_t c)
		{
			if (env_on_)	return std::make_pair(STATE_VERBATIM_, false);

			switch (c) {
			case '\n':
				return std::make_pair(state_, true);

			case ' ': case '\t':
				//	ommit white spaces
				return std::make_pair(state_, true);

			case '.':
				if (col_ == 1)	return std::make_pair(STATE_NL_DOT_, true);
				else return std::make_pair(STATE_WS_DOT_, true);

			case '>':
			case '<':
			case '&':
			case '"':
			case '\'':
				//	HTML entities
				emit(symbol(SYM_CHAR, c));
				return std::make_pair(state_, true);

			case U'¶':	//	0xb6
				emit(symbol(SYM_FUN_PAR, u8"¶"));
				return std::make_pair(state_, true);

			case std::numeric_limits<std::uint32_t>::max():	//	eof
				return std::make_pair(state_, true);

			default:
				//
				//	accept all other as text
				//
				break;
			}
			return std::make_pair(STATE_TEXT_, false);
		}

		std::pair<lexer::state, bool> lexer::state_error(std::uint32_t c)
		{
			switch (c) {
			case U'¶':	//	0xb6
				emit(symbol(SYM_FUN_PAR, u8"¶"));
				return std::make_pair(pop(STATE_START_), true);
			default:
				break;
			}

			return std::make_pair(state_, true);
		}

		std::pair<lexer::state, bool> lexer::skip_ws(std::uint32_t c)
		{
			switch (c)
			{
			case '\n':
			case ' ':
			case '\t':
				//	ommit white spaces
				return std::make_pair(state_, true);
			default:
				break;
			}

			BOOST_ASSERT_MSG(state_stack_.size() > 1, "insuffient lexer stack size (skip)");
			BOOST_ASSERT_MSG(state_stack_.top() != state_, "potential loop (skip)");

			//
			//	restore lexer state and reject symbol c
			//
			return std::make_pair(pop(), false);
		}

		std::pair<lexer::state, bool> lexer::state_dot(std::uint32_t c)
		{
			switch (c) {
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
				tmp_ += '0';
				tmp_ += '.';
				return std::make_pair(STATE_NUMBER_, false);

			case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': 
			case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z': 
			case '_':
				return std::make_pair(save(STATE_TOKEN_), false);

			default:
				//	'.' acts like an escape symbol:
				//	'.' + any CHAR == any char 
				emit(symbol(SYM_CHAR, c));
				break;
			}
			return std::make_pair(STATE_START_, true);
		}

		std::pair<lexer::state, bool> lexer::state_number(std::uint32_t c)
		{
			switch (c) {
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
				tmp_ += c;
				break;
			case '\n':
			default:
				emit_tmp(SYM_NUMBER);
				return std::make_pair(pop(STATE_START_), false);
			}

			return std::make_pair(state_, true);
		}

		std::pair<lexer::state, bool> lexer::state_token(std::uint32_t c)
		{
			switch (c) {
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
			case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm':
			case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
			case '_': //case '.':
				tmp_ += c;
				break;

			case '.':
				pop();	//	remove STATE_WS_DOT_
				emit_tmp(SYM_FUN_WS);
				return std::make_pair(STATE_FUNCTION_, false);

			case '\n':
			case ' ': case '\t': 
				//	token complete
				switch (top()) {
				case STATE_NL_DOT_:
					pop();
					if (this->env_on_)	emit_tmp(SYM_FUN_ENV);
					else emit_tmp(SYM_FUN_NL);
					return std::make_pair(transit_ws(STATE_FUNCTION_), true);
				case STATE_WS_DOT_:
					pop();
					emit_tmp(SYM_FUN_WS);
					return std::make_pair(transit_ws(STATE_FUNCTION_), true);

				default:
					err_(cyng::logging::severity::LEVEL_ERROR, get_state_name(state_) + ": invalid end of function call");
					break;
				}
				return std::make_pair(pop(), true);

			case '(':
				//	token complete
				switch (top()) {
				case STATE_NL_DOT_:
					pop();	//	remove STATE_NL_DOT_
					BOOST_ASSERT(complete());
					if (this->env_on_) {
						if (boost::algorithm::equals(U"end", tmp_)) {
							this->env_on_ = false;
						}
						emit_tmp(SYM_FUN_ENV);
					}
					else {
						emit_tmp(SYM_FUN_NL);
					}
					return std::make_pair(STATE_FUNCTION_, false);
				case STATE_WS_DOT_:
					pop();	//	remove STATE_WS_DOT_
					emit_tmp(SYM_FUN_WS);
					return std::make_pair(STATE_FUNCTION_, false);

				default:
					err_(cyng::logging::severity::LEVEL_ERROR, get_state_name(state_) + ": invalid end of function call");
					break;
				}
				return std::make_pair(pop(), false);

			case '-':
				if (boost::algorithm::equals(U"start", tmp_)) {

					//
					//	"start" is a keyword
					//	open environment
					//	all characters after '-' define a "boundary" that has to match with the ".end-" parameter
					//
					env_open_.clear();
					BOOST_ASSERT(!env_on_);
					env_on_ = true;
					err_(cyng::logging::severity::LEVEL_TRACE, get_state_name(state_) + ": open environment");
					return std::make_pair(save(STATE_ENV_OPEN_), true);
				}
				else if (boost::algorithm::equals(U"end", tmp_)) {
					err_(cyng::logging::severity::LEVEL_TRACE, get_state_name(state_) + ": close environment");
					env_close_.clear();
					return std::make_pair(save(STATE_ENV_CLOSE_), true);
				}
				else {
					err_(cyng::logging::severity::LEVEL_ERROR, get_state_name(state_) + ": '-' is an invalid character for function names");
					tmp_ += c;
				}
				break;

			default:
				break;
			}
			return std::make_pair(state_, true);
		}

		std::pair<lexer::state, bool> lexer::state_function(std::uint32_t c)
		{
			switch (c) {
			case '(':
				return std::make_pair(STATE_PARAMS_, true);
			case ' ': case '\t':
				err_(cyng::logging::severity::LEVEL_ERROR, get_state_name(state_) + ": no SPACE expected");
				//return std::make_pair(STATE_1_ARG_, true);
				emit_tmp(SYM_VALUE);
				emit(symbol(SYM_FUN_CLOSE, ')'));	//	virtual ')'
				return std::make_pair(pop(STATE_START_), true);

			case '.':
				//	nested function call as argument for STATE_1_ARG_
				err_(cyng::logging::severity::LEVEL_WARNING, get_state_name(state_) + ": nested call for single argument function");
				return std::make_pair(save(STATE_WS_DOT_), true);
				
			default:
				break;
			}
			return std::make_pair(STATE_1_ARG_, false);
		}

		std::pair<lexer::state, bool> lexer::state_1_arg(std::uint32_t c)
		{
			switch (c) {
			case '\n': case U'¶':	//	0xb6
			case ' ': case '\t': 
				emit_tmp(SYM_VALUE);
				emit(symbol(SYM_FUN_CLOSE, ')'));	//	virtual ')'
				return std::make_pair(pop(STATE_START_), true);

			case '(': case ')':
				err_(cyng::logging::severity::LEVEL_ERROR, get_state_name(state_) + ": brackets - could be an error");
			default:
				tmp_ += c;
				break;
			}
			return std::make_pair(state_, true);
		}

		std::pair<lexer::state, bool> lexer::state_params(std::uint32_t c)
		{
			//
			//	have to wait for ':' or WS to detect an ARG or KEY/VALUE syntax
			//
			switch (c) {
			case ':':
				//	key/value syntax
				emit_tmp(SYM_KEY);
				return std::make_pair(transit_ws(save(STATE_VALUE_)), true);

			case '"':
				if (tmp_.empty()) {
					//
					//	start a quote
					//
					return std::make_pair(save(STATE_QUOTE_), true);
				}
				tmp_ += c;
				break;

			case '\n':
			case ' ': case '\t':
				emit_tmp(SYM_VALUE);

				//
				//	remain in state STATE_PARAMS_ means that single ARGS and
				//	key/value pairs could be mixed
				//
				return std::make_pair(transit_ws(state_), true);

			case ')':
				emit_tmp(SYM_VALUE);
				emit(symbol(SYM_FUN_CLOSE, ')'));
				if (complete()) {
					//
					//	function call complete. state stack is empty
					//
					err_(cyng::logging::severity::LEVEL_TRACE, get_state_name(state_) + ": function call complete");
					return std::make_pair(STATE_TRAIL_, true);

				}
				return std::make_pair(pop(STATE_START_), true);

			case '.':
				if (tmp_.empty()) {
					//
					//	function as argument
					//
					return std::make_pair(save(STATE_WS_DOT_), true);
				}
				//
				//	'.' are allowed in arguments and key names
				//
			default:
				tmp_ += c;
				break;
			}
			return std::make_pair(state_, true);
		}

		std::pair<lexer::state, bool> lexer::state_value(std::uint32_t c)
		{
			switch (c) {
			case ')':
				//
				//	key/value pair complete
				//
				emit_tmp(SYM_VALUE);
				return std::make_pair(pop(), false);

			case ',':
				//
				//	key/value pair complete, but more
				//	paremeters are coming
				//
				emit_tmp(SYM_VALUE);
				return std::make_pair(transit_ws(pop()), true);

			//	special case numbers
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
				if (tmp_.empty()) {
					return std::make_pair(save(STATE_NUMBER_), false);
				}
				break;

			case '"':
				if (tmp_.empty()) {
					//
					//	start a quote
					//
					return std::make_pair(save(STATE_QUOTE_), true);
				}
				break;

			case '.':
				if (tmp_.empty()) {
					//
					//	values could be result of functions too
					//
					return std::make_pair(save(STATE_WS_DOT_), true);
				}
				break;

			case '(':
				if (!tmp_.empty()) {
					//
					//	function name didn't start with a '.' 
					//	Is this an error or syntactic sugar?
					//	Because it's unambiguous it's syntactic sugar.
					//
					push();
					return std::make_pair(save(STATE_WS_DOT_), true);
				}
				break;

			case '\n':
			case ' ': case '\t':
				//
				//	ignore white spaces in values
				//
				return std::make_pair(state_, true);

			default:
				break;
			}

			tmp_ += c;
			return std::make_pair(state_, true);
		}

		std::pair<lexer::state, bool> lexer::state_quote(std::uint32_t c)
		{
			switch (c)
			{
			case '\n':
				err_(cyng::logging::severity::LEVEL_ERROR, get_state_name(state_) + ": no line breaks allowed");

				//
				//	fall through
				//
			case '"':
			case std::numeric_limits<std::uint32_t>::max():	//	eof
				//
				//	ToDo: we have no history to decide if this is a value
				//	or an argument. Maybe mixing of arguments and parameters
				//	is no good idea or additional information on state stack
				//	is required.
				//
				//emit_tmp(SYM_VALUE);
				return std::make_pair(pop(), true);
			default:
				break;
			}

			tmp_ += c;
			return std::make_pair(state_, true);
		}

		std::pair<lexer::state, bool> lexer::state_text(std::uint32_t c)
		{
			switch (c)
			{
			case '\n':
			case ' ': case '\t':
			case std::numeric_limits<std::uint32_t>::max():	//	eof
				emit_tmp(SYM_WORD);
				return std::make_pair(STATE_START_, true);

			case '.': case '(': case ')':
			case ',': case ':': case '!':
			case '?': case ';':
				//
				//	characters that terminate a word
				//
				emit_tmp(SYM_WORD);
				emit(symbol(SYM_CHAR, c));
				break;

 			case U'¶':	//	0xb6
				emit(symbol(SYM_FUN_PAR, u8"¶"));
				return std::make_pair(STATE_START_, true);

			default:
				tmp_ += c;
				break;
			}

			return std::make_pair(state_, true);
		}

		std::pair<lexer::state, bool> lexer::state_env_open(std::uint32_t c)
		{
			//
			//	empty boundaries allowed
			//
			switch (c)
			{
			case '\n':
			case ' ': case '\t':
			case std::numeric_limits<std::uint32_t>::max():	//	eof
				return std::make_pair(pop(), true);

			case '(':
				return std::make_pair(pop(), false);

			default:
				env_open_ += c;
				break;
			}
			return std::make_pair(state_, true);
		}

		std::pair<lexer::state, bool> lexer::state_env_close(std::uint32_t c)
		{
			switch (c)
			{
			case '\n':
			case ' ': case '\t':
			case std::numeric_limits<std::uint32_t>::max():	//	eof
				if (!boost::algorithm::equals(env_open_, env_close_)) {
					err_(cyng::logging::severity::LEVEL_ERROR, get_state_name(state_) + ": environment boundary doesn't match");
				}
				return std::make_pair(pop(), true);

			case '(':
				if (!boost::algorithm::equals(env_open_, env_close_)) {
					err_(cyng::logging::severity::LEVEL_ERROR, get_state_name(state_) + ": environment boundary doesn't match");
				}
				BOOST_ASSERT(env_on_);
				return std::make_pair(pop(), false);

			default:
				env_close_ += c;
				break;
			}
			return std::make_pair(state_, true);
		}

		std::pair<lexer::state, bool> lexer::state_verbatim(std::uint32_t c)
		{
			switch (c)
			{
			case '.':
				if (col_ == 1) {
					emit_tmp(SYM_WORD);
					return std::make_pair(STATE_NL_DOT_, true);
				}
				break;

			default:
				break;
			}

			tmp_ += c;
			return std::make_pair(state_, true);
		}

		std::pair<lexer::state, bool> lexer::state_trail(std::uint32_t c)
		{
			switch (c)
			{
			case '\n':
				emit(symbol(SYM_FUN_PAR, u8"¶"));

				//
				//	fall through
				//

			case U'¶':	//	0xb6
				return std::make_pair(STATE_START_, false);

			case ' ': case '\t':
				break;

			case ')':
				err_(cyng::logging::severity::LEVEL_WARNING, get_state_name(state_) + ": function already closed");
				emit(symbol(SYM_CHAR, c));
				break;

			case std::numeric_limits<std::uint32_t>::max():	//	eof
				return std::make_pair(STATE_START_, true);
				break;

			default:
				emit(symbol(SYM_CHAR, c));
				break;
			}
			return std::make_pair(STATE_START_, true);
		}

		std::string lexer::get_state() const
		{
			return get_state_name(state_);
		}

		lexer::state lexer::save(lexer::state save_state, lexer::state ret_state)
		{
			push(save_state);
			return ret_state;
		}

		lexer::state lexer::save(state s)
		{
			return save(state_, s);
		}

		void lexer::push()
		{
			push(state_);
		}

		void lexer::push(lexer::state s)
		{
			state_stack_.push(s);
		}

		lexer::state lexer::pop()
		{
			if (state_stack_.empty())
			{
				return STATE_ERROR_;
			}
			auto state = state_stack_.top();
			state_stack_.pop();
			return state;
		}

		lexer::state lexer::pop(state s)
		{
			return (complete())
				? s
				: pop()
				;
		}


		lexer::state lexer::top() const
		{
			return (state_stack_.empty())
				? STATE_ERROR_
				: state_stack_.top()
				;
		}

		lexer::state lexer::transit_ws(state s)
		{
			push(s);
			return STATE_SKIP_WS_;
		}


		std::string lexer::dump_stack() const
		{
			struct hacked_stack : public std::stack<state>
			{
				hacked_stack(std::stack<state> const& s)
					: std::stack<state>(s)
				{}
				static std::deque<state> get_container(std::stack<state> const& s)
				{
					const hacked_stack hs(s);
					return hs.c;
				}
			};

			auto cont = hacked_stack::get_container(state_stack_);
			std::string result;
			if (!cont.empty()) {
				for (auto pos = ++cont.begin(); pos != cont.end(); ++pos)
				{
					result += get_state_name(*pos);
					result += "=>";
				}
				return result;
			}

			return "***error: stack is empty";
		}

	}	//	docscript
}	//	cyng


