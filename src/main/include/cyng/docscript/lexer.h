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
#include <cyng/log/severity.h>
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
			lexer(emit_symbol_f, std::function<void(cyng::logging::severity, std::string)>);

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
				STATE_SKIP_WS_,
				STATE_START_,
				STATE_NL_DOT_,		//	'.' after NL
				STATE_WS_DOT_,		//	'.' after WS
				STATE_NUMBER_,		//	0 ... 9
				STATE_TOKEN_,		//	lowercase characters and '_'
				STATE_FUNCTION_,	//	expection arguments 
				STATE_1_ARG_,		//	function with one (simple) argument
				STATE_PARAMS_,		//	parameters enclosed (...)
				STATE_VALUE_,		//	read value of a key
				STATE_QUOTE_,		//	string enclosed in "
				STATE_TEXT_,		//	text and punctuation
				STATE_ENV_OPEN_,	//	read environment boundary
				STATE_ENV_CLOSE_,	//	compare environment boundary
				STATE_VERBATIM_,	//	environment data
				STATE_TRAIL_,		//	after function call ")..."

			}	state_;

			/**
			 * emitter for complete symbols
			 */
			emit_symbol_f	emit_;
			std::function<void(cyng::logging::severity, std::string)>	err_;

			/**
			 * temporary buffer
			 */
			utf::u32_string tmp_;
			utf::u32_string env_open_, env_close_;

			/**
			 * additional state info to collect
			 * an environment
			 */
			bool env_on_;

			/**
			 * state stack
			 */
			std::stack<state>	state_stack_;

			/**
			 * virtual line position
			 */
			std::size_t col_;

		private:
			std::pair<state, bool> state_start(std::uint32_t);
			std::pair<state, bool> state_error(std::uint32_t);
			std::pair<state, bool> skip_ws(std::uint32_t c);

			std::pair<state, bool> state_dot(std::uint32_t);
			std::pair<state, bool> state_number(std::uint32_t);
			std::pair<state, bool> state_token(std::uint32_t);
			std::pair<state, bool> state_function(std::uint32_t);
			std::pair<state, bool> state_1_arg(std::uint32_t);
			std::pair<state, bool> state_params(std::uint32_t);
			std::pair<state, bool> state_value(std::uint32_t);
			std::pair<state, bool> state_quote(std::uint32_t);
			std::pair<state, bool> state_text(std::uint32_t);
			std::pair<state, bool> state_env_open(std::uint32_t);
			std::pair<state, bool> state_env_close(std::uint32_t);
			std::pair<state, bool> state_verbatim(std::uint32_t);
			std::pair<state, bool> state_trail(std::uint32_t);

			bool emit_tmp(symbol_type);
			void emit(symbol&&) const;

			/**
			 * Removes the element on top of the stack and returns the value
			 */
			state pop();

			/**
			 * if state stack is empty function returns the given
			 * parameter s. Otherwise it works like pop()
			 */
			state pop(state s);

			/**
			 * Push current state on state stack
			 */
			void push();
			void push(state);

			/**
			 * saves the current state and returns the specified state
			 */
			state save(state);
			state save(state, state);
			state transit_ws(state);
			state top() const;

			std::string get_state_name(state) const;

		};

	}	//	docscript
}	//	cyng

#endif	//	CYNG_DOCSCRIPT_LEXER_H




