/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_DOCSCRIPT_COMPILER_H
#define CYNG_DOCSCRIPT_COMPILER_H

#include <cyng/docscript/symbol.h>
#include <cyng/docscript/library.h>
#include <cyng/object.h>
#include <cyng/intrinsics/sets.h>

#include <chrono>
#include <boost/filesystem.hpp>

namespace cyng	
{
	namespace docscript
	{
		class compiler;
		vector_t move_program(compiler& c);

		/**
		 * shift/reduce compiler
		 */
		class compiler
		{
			friend vector_t move_program(compiler& c);

			using fp = std::shared_ptr<function const>;

			/**
			 * Call frame contains parameters (key/value pairs) or an argument list
			 */
			enum call_frame_type {
				PARAMETERS,
				ARGUMENT_LIST
			};

			class call_frame
			{
			public:
				call_frame(compiler::fp, compiler&, std::size_t depth, call_frame_type cft);
				call_frame(call_frame&&);
				virtual ~call_frame();

			public:
				compiler::fp fp_;

			private:
				compiler& compiler_;
				std::size_t const depth_;	//!<	call depth
				std::size_t const pos_;		//!<	programm position

				/**
				 * Call frame contains parameters (key/value pairs) or an argument list
				 */
				bool const has_params_;		
			};

		public:
			compiler(std::list<symbol> const&, int verbose);

			void run(boost::filesystem::path out, bool meta);

		private:
			/**
			 * Add prolog 
			 */
			void init();

			/**
			 * Check symbol type and produce next symbol
			 */
			bool match(symbol_type st);

			/**
			 * Compile until EOF or .end()
			 *
			 * @param depth recursion depth
			 * @return true if EOF
			 */
			bool loop(std::size_t depth);

			/**
			 * @param name function name
			 * @param depth call depth
			 * @param nl true for NL functions otherwise false
			 */
			void func(std::string name, std::size_t depth, bool nl);

			/**
			 * new paragraph
			 */
			void fun_par(std::size_t);
			void key(std::string name, bool, std::string key, std::size_t depth);

			/**
			 * @param name function name
			 * @return argument count
			 */
			std::size_t arg(std::string name, std::size_t depth);

			void init_library();

			/**
			 * @return a function that produces meta data of 
			 * a function call
			 */
			fp lookup(std::string const& name) const;

			void env_raw();
			void env_dsl(std::string);

		public:
			param_map_t meta_;

		private:
			/**
			 * symbol source
			 */
			symbol_reader producer_;
			symbol const* look_ahead_;
			int const verbose_;

			/**
			 * function library
			 */
			function_map	library_;

			/**
			 * generated program
			 */
			vector_t	prg_;

			/**
			 * position in source file
			 */
			std::uint32_t line_,
				column_;

			/**
			 * source file name
			 */
			std::string source_file_;

		};

		cyng::object produce_value(std::string);
		cyng::object produce_number(std::string);

	}	//	docscript
}	//	cyng

#endif	//	CYNG_DOCSCRIPT_COMPILER_H




