/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_DOCSCRIPT_LEXER_H
#define CYNG_DOCSCRIPT_LEXER_H

#include <cyng/docscript/token.h>
#include <cyng/docscript/symbol.h>
#include <cyng/intrinsics/sets.h>
#include <fstream>
#include <stack>
#include <boost/uuid/uuid.hpp>

namespace cyng	
{
	namespace docscript
	{
		/**
		* Build symbols and store environments on disk.
		*/
		class lexer
		{
		public:
			lexer(emit_symbol_f);

			/**
			 * process next character.
			 *
			 * @return false to reject symbol c
			 */
			bool next(std::uint32_t c);

			/**
			 * self test
			 */
			bool complete();

			/**
			 * debug
			 */
			std::string get_state() const;
			std::string dump_stack() const;

		private:
			enum state
			{
				STATE_ERROR_,
				STATE_START_,
				STATE_NL_,	//	
				STATE_PAR_,	//	after multiple NLs
				STATE_DOT_NL_,		//	'.' after NL
				STATE_DOT_WS_,		//	'.' after WS
				STATE_FUN_NAME_,	//	function name
				STATE_ARG_,			//	read 1 arg
				STATE_FUN_OPEN_,	//	function open
				STATE_FUN_CLOSE_,	//	function complete ')'
				STATE_ENV_OPEN_,	//	.+
				STATE_ENV_CLOSE_,	//	.-
				STATE_ENV_,
				STATE_ENV_EOL_,		//	EOL in environment
				STATE_BOUNDARY_,	//	read the boundary string
				STATE_DECIMAL_,		//	decial number 0 .. 9
				STATE_TXT_,			//	running paragraph
				STATE_QUOTE_,		//	quoted text
				STATE_PARAM_,		//	expecting function arguments
				STATE_KEY_,			//	pending key/arg decision
				STATE_VALUE_,		//	a parameter value
				STATE_NUMBER_,		//	a parameter number
				STATE_SKIP_WS_,		//	skip white spaces
			}	state_;

			/**
			 * emitter for complete symbols
			 */
			emit_symbol_f	emit_;

			/**
			 * temporary buffer
			 */
			utf::u32_string tmp_;

			/**
			 * state stack
			 */
			std::stack<state>	state_stack_;

		private:
			std::pair<state, bool> state_start(std::uint32_t);
			state state_nl(std::uint32_t);
			std::pair<state, bool> state_paragraph(std::uint32_t);
			std::pair<state, bool> state_dot_nl(std::uint32_t);
			std::pair<state, bool> state_dot_ws(std::uint32_t);
			std::pair<state, bool> state_fun_name(std::uint32_t);
			state state_arg(std::uint32_t);
			state state_fun_open(std::uint32_t);
			state state_fun_close(std::uint32_t);
			std::pair<state, bool> state_decimal(std::uint32_t c);
			state state_txt(std::uint32_t);
			state state_quote(std::uint32_t);
			state state_param(std::uint32_t);
			state state_key(std::uint32_t);
			std::pair<state, bool> state_value(std::uint32_t);
			std::pair<state, bool> state_number(std::uint32_t);
			state state_env_open(std::uint32_t);
			state state_env_close(std::uint32_t);
			state state_env(std::uint32_t);
			state state_env_eol(std::uint32_t);
			std::pair<state, bool> skip_ws(std::uint32_t);

			bool emit_tmp(symbol_type);
			void emit(symbol&&) const;
			state pop();
			void push();
			void push(state);

			/**
			 * saves the current state and returns the specified state
			 */
			state save(state);
			state save(state, state);
			state transit_ws(state);

			std::string get_state_name(state) const;

		};

	}	//	docscript
}	//	cyng

#endif	//	CYNG_DOCSCRIPT_LEXER_H




