/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/docscript/lexer.h>
#include <boost/assert.hpp>

namespace cyng	
{
	namespace docscript
	{

		lexer::lexer(emit_symbol_f f)
			: state_(STATE_START_)
			, emit_(f)
			, tmp_()
			, state_stack_()
		{
			state_stack_.push(STATE_ERROR_);
		}

		bool lexer::complete()
		{
			if (state_stack_.size() != 1)
			{
				return false;
			}
			return true;
		}

		bool lexer::next(std::uint32_t c)
		{
			bool advance = true;
			switch (state_)
			{
			case STATE_START_:
				std::tie(state_, advance) = state_start(c);
				break;
			case STATE_ERROR_:
				break;
			case STATE_NL_:
				state_ = state_nl(c);
				break;
			case STATE_PAR_:
				std::tie(state_, advance) = state_paragraph(c);
				break;
			case STATE_DOT_NL_:
				std::tie(state_, advance) = state_dot_nl(c);
				break;
			case STATE_DOT_WS_:
				std::tie(state_, advance) = state_dot_ws(c);
				break;
			case STATE_FUN_NAME_:
				std::tie(state_, advance) = state_fun_name(c);
				break;
			case STATE_ARG_:
				state_ = state_arg(c);
				break;
			case STATE_FUN_OPEN_:
				state_ = state_fun_open(c);
				break;
			case STATE_PARAM_:
				state_ = state_param(c);
				break;
			case STATE_KEY_:
				state_ = state_key(c);
				break;
			case STATE_VALUE_:
				std::tie(state_, advance) = state_value(c);
				break;
			case STATE_NUMBER_:
				std::tie(state_, advance) = state_number(c);
				break;
			case STATE_ENV_OPEN_:
				state_ = state_env_open(c);
				break;
			case STATE_ENV_CLOSE_:
				state_ = state_env_close(c);
				break;
			case STATE_ENV_:
				state_ = state_env(c);
				break;
			case STATE_ENV_EOL_:
				state_ = state_env_eol(c);
				break;
			case STATE_BOUNDARY_:
				std::tie(state_, advance) = std::make_pair(STATE_NL_, false);
				break;
				//case STATE_NUM_:
				//	state_ = state_num(c);
				//	break;
			case STATE_DECIMAL_:
				std::tie(state_, advance) = state_decimal(c);
				break;
			case STATE_TXT_:
				state_ = state_txt(c);
				break;
			case STATE_QUOTE_:
				state_ = state_quote(c);
				break;
			case STATE_SKIP_WS_:
				std::tie(state_, advance) = skip_ws(c);
				break;
			default:
				std::cerr
					<< "***error: internal lexer error at "
					//<< get_position(tok)
					<< std::endl
					;
				state_ = STATE_ERROR_;
				break;
			}

			return advance;
		}

		std::pair<lexer::state, bool> lexer::state_start(std::uint32_t c)
		{
			switch (c)
			{
			case '\n':
				return std::make_pair(STATE_NL_, true);
			case '.':
				push(STATE_START_);
				return std::make_pair(STATE_DOT_WS_, true);
			case ' ':
				return std::make_pair(state_, true);
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				return std::make_pair(save(STATE_TXT_, STATE_DECIMAL_), false);
			case '>':
			case '<':
			case '&':
			case '"':
			case '\'':
				//	HTML entities
				emit(symbol(SYM_CHAR, c));
				return std::make_pair(state_, true);
			default:
				BOOST_ASSERT(tmp_.empty());
				tmp_ += c;
				break;
			}
			return std::make_pair(STATE_TXT_, true);
		}

		lexer::state lexer::state_nl(std::uint32_t c)
		{
			switch (c)
			{
			case '\n':
				return STATE_PAR_;
			case '.':
				return save(STATE_DOT_NL_);
			case ' ':
			case '\t':
				return state_;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				tmp_ += c;
				return save(STATE_TXT_, STATE_DECIMAL_);
			default:
				tmp_ += c;
				break;
			}
			return STATE_TXT_;
		}

		std::pair<lexer::state, bool>  lexer::state_paragraph(std::uint32_t c)
		{
			switch (c)
			{
			case '.':
				push(STATE_NL_);
				return std::make_pair(STATE_DOT_NL_, true);
			case '\t':
			case ' ':
			case '\n':
				return std::make_pair(state_, true);
			default:
				break;
			}

			//	Pilcrow, CAPITULUM
			emit(symbol(SYM_FUN_PAR, u8"¶"));

			//
			//	process symbol c again in other lexer state
			//
			return std::make_pair(STATE_START_, false);
		}

		std::pair<lexer::state, bool>  lexer::state_dot_nl(std::uint32_t c)
		{
			//
			//	Each transition to state DOT_NL saves the current lexer state.
			//	So if no function name is detected, the state has to be popped off.
			//
			switch (c)
			{
			case '+':
				BOOST_ASSERT_MSG(tmp_.empty(), "buffer not empty");
				BOOST_ASSERT_MSG(!state_stack_.empty(), "state stack is empty");
				BOOST_ASSERT_MSG(state_stack_.top() == STATE_NL_, "wrong state");
				pop();
				tmp_ += c;	//	function name
				push(STATE_ENV_);	//	the state after the closed bracket
				return std::make_pair(STATE_FUN_NAME_, true);
			case ' ':
			case '\t':
			case '.':
				//
				//	".." => "."
				//
			case '(':
			case '-':
			case ';':
			case ')':
			case '#':
				//	'special characters
				emit(symbol(SYM_CHAR, c));
				return std::make_pair(pop(), true);
			case '\n':
				emit(symbol(SYM_CHAR, 0x2424));
				return std::make_pair(pop(), true);
			default:
				//
				//	every other symbol is allowd as function name
				//
				break;
			}
			return std::make_pair(STATE_FUN_NAME_, false);
		}

		std::pair<lexer::state, bool> lexer::state_dot_ws(std::uint32_t c)
		{
			switch (c)
			{
			case ' ':
			case '\t':
			case '.':
				//
				//	".." => "."
				//
			case '(':
			case '-':
			case ';':
			case ')':
			case '#':
				//	'special characters
				emit(symbol(SYM_CHAR, c));
				return std::make_pair(pop(), true);
			case '\n':
				//
				//	last symbol was a dot (.) that started a function. But after a new line
				//	it is interpreted as SYM_CHAR.
				emit(symbol(SYM_CHAR, '.'));
				//emit(symbol(SYM_CHAR, 0x2424));
				return std::make_pair(pop(), true);
			default:
				//
				//	every other symbol is allowd as function name
				//
				break;
			}
			return std::make_pair(STATE_FUN_NAME_, false);
		}

		std::pair<lexer::state, bool>  lexer::state_fun_name(std::uint32_t c)
		{
			BOOST_ASSERT_MSG(state_stack_.size() > 1, "insuffient lexer stack size");

			//
			//	read function name
			//
			switch (c)
			{
			case ' ':
			case '\t':
			case '\n':
				switch (state_stack_.top())
				{
				case STATE_NL_:
					emit_tmp(SYM_FUN_NL);
					//
					//	transition to "1 arg" state
					//
					return std::make_pair(transit_ws(STATE_ARG_), true);
				case STATE_START_:
				case STATE_ARG_:
					emit_tmp(SYM_FUN_WS);
					//
					//	transition to "1 arg" state
					//
					return std::make_pair(transit_ws(STATE_ARG_), true);
				case STATE_BOUNDARY_:
					emit_tmp(SYM_FUN_NL);
					emit(symbol(SYM_FUN_CLOSE, ')'));	//	no boundary
					pop();
					return std::make_pair(STATE_NL_, true);
				case STATE_ENV_:
					//emit_tmp(SYM_FUN_NL);
					//return std::make_pair(STATE_NL_, true);
				default:
					std::cerr
						<< "***error: unexpected white space "
						//<< get_position(tok)
						<< std::endl
						;
					break;
				}
				break;
			case '(':
				switch (state_stack_.top())
				{
				case STATE_NL_:
					emit_tmp(SYM_FUN_NL);
					return std::make_pair(transit_ws(STATE_FUN_OPEN_), true);
				case STATE_START_:
				case STATE_VALUE_:
				case STATE_ARG_:
					emit_tmp(SYM_FUN_WS);
					return std::make_pair(transit_ws(STATE_FUN_OPEN_), true);
				case STATE_ENV_:
					emit_tmp(SYM_FUN_NL);
					return std::make_pair(transit_ws(STATE_FUN_OPEN_), true);
				case STATE_BOUNDARY_:
					emit_tmp(SYM_FUN_NL);
					return std::make_pair(transit_ws(STATE_FUN_OPEN_), true);
				default:
					std::cerr
						<< "***error: internal lexer error at "
						//<< get_position(tok)
						<< std::endl
						;
					break;
				}
				break;
			case '"':
				return std::make_pair(save(STATE_ARG_, STATE_QUOTE_), true);
			case '.':
				//
				//	'.' is allowed as part of function names
				//
			default:
				tmp_ += c;
				break;
			}
			return std::make_pair(state_, true);
		}

		lexer::state lexer::state_arg(std::uint32_t c)
		{
			switch (c)
			{
			case ' ':
			case '\t':
			case '\n':
			case std::numeric_limits<std::uint32_t>::max():
				emit_tmp(SYM_ARG);
				while (state_stack_.top() == STATE_ARG_)
				{
					//
					//	close nth argument
					//
					emit(symbol(SYM_FUN_CLOSE, ')'));
					pop();
				}
				emit(symbol(SYM_FUN_CLOSE, ')'));
				return pop();
			case '"':
				return save(STATE_QUOTE_);
			case '.':
				if (tmp_.empty())
				{
					//	chained function call
					return save(STATE_DOT_WS_);
				}
				//
				//	'.' is part of argument value
				//
				tmp_ += c;
				break;
			default:
				tmp_ += c;
				break;
			}
			return state_;
		}

		lexer::state lexer::state_fun_open(std::uint32_t c)
		{
			switch (c)
			{
			case ')':
				emit_tmp(SYM_ARG);
				emit(symbol(SYM_FUN_CLOSE, ')'));
				//	use state stack
				return pop();
			case ' ':
			case '\t':
			case '\n':
				return state_;
			case '"':
				push(STATE_FUN_OPEN_);
				return STATE_QUOTE_;
			case '.':
				push();	//	nested call
				return STATE_DOT_WS_;
			default:
				tmp_ += c;
				return STATE_PARAM_;
			}
			return state_;
		}

		std::pair<lexer::state, bool> lexer::state_decimal(std::uint32_t c)
		{
			switch (c)
			{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				tmp_ += c;
				return std::make_pair(state_, true);
			default:
				break;
			}
			return std::make_pair(pop(), false);
		}

		lexer::state lexer::state_txt(std::uint32_t c)
		{
			switch (c)
			{
			case '\n':
				emit_tmp(SYM_WORD);
				return STATE_NL_;
			case ' ':
			case '\t':
				//
				//	ToDo: additional state required in order to distinguish
				//	dots after a word and dots after a white space.
				//
				emit_tmp(SYM_WORD);
				return STATE_START_;
			case '.':
			case '(':
			case ')':
				emit_tmp(SYM_WORD);
				emit(symbol(SYM_CHAR, c));
				break;
			case std::numeric_limits<std::uint32_t>::max():
				//	eof
				emit_tmp(SYM_WORD);
				return STATE_START_;
			default:
				tmp_ += c;
				break;
			}
			return state_;
		}

		lexer::state lexer::state_quote(std::uint32_t c)
		{
			switch (c)
			{
			case '"':
			case '\n':
			case std::numeric_limits<std::uint32_t>::max():
				switch (state_stack_.top())
				{
				case STATE_FUN_OPEN_:
					emit_tmp(SYM_ARG);
					break;
				case STATE_ARG_:
					emit_tmp(SYM_ARG);
					pop();
					emit(symbol(SYM_FUN_CLOSE, ')'));
					return pop();
				case STATE_VALUE_:
					emit_tmp(SYM_VALUE);
					break;
				default:
					emit_tmp(SYM_WORD);
					break;
				}
				return pop();
			default:
				tmp_ += c;
				break;
			}
			return state_;
		}

		lexer::state lexer::state_param(std::uint32_t c)
		{
			switch (c)
			{
			case ':':
				//	key should not be empty
				emit_tmp(SYM_KEY);
				return transit_ws(STATE_VALUE_);
			case ' ':
			case '\t':
			case '\n':
				//	additional lexer state required to distinguish
				//	between an ARG and an ':' after the white space(s).
				return STATE_KEY_;

#ifdef _CYNG_DOCSCRIPT_NOT_DOT_REQUIRED
				//
				//	to accept functions without a preceding '.' is not a good idea
				//	since every open opening '(' in a text is virtual a function what makes
				//	no sense.
				//
			case '(':
				//
				//	accept function names without a preceding . (DOT)
				//
				push(STATE_FUN_OPEN_);	//	overwrite state
				emit(symbol(SYM_FUN_WS, tmp_));
				tmp_.clear();
				return STATE_FUN_OPEN_;
#endif
			case ')':
				emit_tmp(SYM_ARG);
				emit(symbol(SYM_FUN_CLOSE, ')'));
				return pop();
			case '"':
				push(STATE_NL_);
				return STATE_QUOTE_;
			default:
				tmp_ += c;
				break;
			}
			return state_;
		}

		lexer::state lexer::state_key(std::uint32_t c)
		{
			switch (c)
			{
			case ':':
				//	key should not be empty
				emit_tmp(SYM_KEY);
				return STATE_VALUE_;
			case ' ':
			case '\t':
			case '\n':
				//	ommit white spaces
				break;
			case ')':
				emit_tmp(SYM_ARG);
				emit(symbol(SYM_FUN_CLOSE, ')'));
				return pop();
			case '"':
				push(STATE_NL_);
				return STATE_QUOTE_;
			default:
				emit_tmp(SYM_ARG);
				tmp_ += c;
				break;
			}
			return STATE_PARAM_;
		}

		std::pair<lexer::state, bool> lexer::state_value(std::uint32_t c)
		{
			switch (c)
			{
			case ')':
				emit_tmp(SYM_VALUE);
				emit(symbol(SYM_FUN_CLOSE, ')'));
				return std::make_pair(pop(), true);
			case ',':
				emit_tmp(SYM_VALUE);
				return std::make_pair(STATE_FUN_OPEN_, true);
			case '.':
				push();
				return std::make_pair(STATE_DOT_WS_, true);
			case '(':
				//	function name didn't start with a '.' 
				//	Is this an error or syntactic sugar?
				//	Because it's unambiguous it's syntactic sugar.
				emit_tmp(SYM_VALUE);
				return std::make_pair(STATE_DOT_WS_, true);
			case ' ':
			case '\t':
				//BOOST_ASSERT_MSG(tmp_.empty(), "tmp not empty");
				//std::cerr
				//	<< "***error: white spaces not allowed in values "
				//	//<< get_position(tok)
				//	<< std::endl
				//	;
				//	skip white space
				return std::make_pair(state_, true);
			case '"':
				return std::make_pair(save(STATE_QUOTE_), true);
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if (tmp_.empty())
				{
					return std::make_pair(save(STATE_NUMBER_, STATE_DECIMAL_), false);
				}
				else
				{
					tmp_ += c;
				}
				break;
			default:
				tmp_ += c;
				break;
			}
			return std::make_pair(state_, true);
		}

		std::pair<lexer::state, bool> lexer::state_number(std::uint32_t c)
		{
			//
			//	return from DECIMAL state
			//
			switch (c)
			{
			case ')':
			case std::numeric_limits<std::uint32_t>::max():
				emit_tmp(SYM_NUMBER);
				emit(symbol(SYM_FUN_CLOSE, ')'));
				return std::make_pair(pop(), true);
			case ',':
				emit_tmp(SYM_NUMBER);
				return std::make_pair(STATE_FUN_OPEN_, true);
			case ' ':
			case '\t':
				return std::make_pair(state_, true);
			default:
				break;
			}
			return std::make_pair(STATE_VALUE_, false);
		}

		lexer::state lexer::state_env_open(std::uint32_t c)
		{
			switch (c)
			{
			case '(':
				return STATE_ENV_;
			case ' ':
			case '\t':
			default:
				break;
			}
			return state_;
		}

		lexer::state lexer::state_env(std::uint32_t c)
		{
			//
			//	decomposing symbols
			//
			switch (c) {
			case '\n':
				return STATE_ENV_EOL_;
			case 0x21d0:
				//	0x21d0 "<="
				tmp_ += '<';
				tmp_ += '=';
				return state_;
			case 0x027F9:
				//	0x027F9 "=>"
				tmp_ += '=';
				tmp_ += '>';
				return state_;
			case 0x2260:
				//	0x2260 "!="
				tmp_ += '!';
				tmp_ += '=';
				return state_;
			default:
				break;
			}
			tmp_ += c;
			return state_;
		}

		lexer::state lexer::state_env_eol(std::uint32_t c)
		{
			if (c == '.')
			{
				return STATE_ENV_CLOSE_;
			}

			if (c == '\n')
			{
				tmp_ += c;
				return state_;
			}
			tmp_ += '\n';
			tmp_ += c;
			return STATE_ENV_;
		}

		lexer::state lexer::state_env_close(std::uint32_t c)
		{
			switch (c)
			{
			case '-':
				emit_tmp(SYM_ARG);
				tmp_ += c;
				push(STATE_BOUNDARY_);
				return STATE_FUN_NAME_;
			default:
				tmp_ += '\n';
				tmp_ += '.';
				tmp_ += c;
				break;
			}
			return STATE_ENV_;
		}

		std::pair<lexer::state, bool> lexer::skip_ws(std::uint32_t c)
		{
			switch (c)
			{
			case ' ':
			case '\t':
			case '\n':
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

		lexer::state lexer::pop()
		{
			if (state_stack_.empty())
			{
				return STATE_ERROR_;
			}
			auto state = state_stack_.top();
			state_stack_.pop();
#ifdef _DEBUG
			//std::cout << "POP " << state_stack_.size() << std::endl;
#endif
			return state;
		}

		void lexer::push()
		{
			push(state_);
		}

		void lexer::push(lexer::state s)
		{
			state_stack_.push(s);
#ifdef _DEBUG
			//std::cout << "PUSH " << state_stack_.size() << std::endl;
#endif
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

		lexer::state lexer::transit_ws(state s)
		{
			push(s);
			return STATE_SKIP_WS_;
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
			case STATE_ERROR_:	return "ERROR";
			case STATE_START_:	return "START";
			case STATE_NL_:		return "NL";
			case STATE_PAR_:	return "PAR";
			case STATE_DOT_NL_:	return "DOT_NL";
			case STATE_DOT_WS_:	return "DOT_WS";
			case STATE_FUN_NAME_:	return "FUN_NAME";
			case STATE_ARG_:	return "ARG";
			case STATE_FUN_OPEN_:	return "FUN_OPEN";
			case STATE_ENV_OPEN_:	return "ENV_OPEN";
			case STATE_ENV_CLOSE_:	return "ENV_CLOSE";
			case STATE_ENV_:		return "ENV";
			case STATE_ENV_EOL_:	return "ENV_EOL";
			case STATE_BOUNDARY_:	return "BOUNDARY";
				//case STATE_NUM_:		return "NUM";
			case STATE_DECIMAL_:		return "DECIMAL";
			case STATE_TXT_:		return "TXT";
			case STATE_QUOTE_:		return "QUOTE";
			case STATE_PARAM_:		return "PARAM";
			case STATE_KEY_:		return "KEY";
			case STATE_VALUE_:		return "VALUE";
			case STATE_NUMBER_:		return "NUMBER";
			case STATE_SKIP_WS_:	return "SKIP_WS";
			default:
				break;
			}
			return "unknown lexer state";

		}

		std::string lexer::get_state() const
		{
			return get_state_name(state_);
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
					//return s.*hacked_stack::c;
				}
			};

			auto cont = hacked_stack::get_container(state_stack_);
			std::string result;
			for (auto pos = ++cont.begin(); pos != cont.end(); ++pos)
			{
				result += get_state_name(*pos);
				result += "=>";
			}
			return result;
		}
	}	//	docscript
}	//	cyng


