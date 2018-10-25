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

			class trailer
			{
			public:
				trailer(int, vector_t&, fp);
				trailer(trailer&&);
				virtual ~trailer();

			public:
				fp fp_;
			private:
				const int verbose_;
				vector_t& prg_;
			};

		public:
			compiler(std::list<symbol> const&, int verbose);

			void run(std::chrono::system_clock::time_point const& last_write_time
				, uintmax_t file_size
				, boost::filesystem::path const& out);

		private:
			/**
			* Check symbol type and produce next symbol
			*/
			bool match(symbol_type st);

			/**
			* @param name function name
			*/
			void fun_nl(std::string name);

			/**
			* @param name function name
			*/
			void fun_ws(std::string name);

			/**
			 * new paragraph
			 */
			void fun_par(trailer&&);
			void key(std::string name, bool, std::string key);
			void arg(std::string name, bool, std::string value);

			void init_library();

			/**
			 * @return a function that produces meta data of 
			 * a function call
			 */
			fp lookup(std::string const& name) const;

			trailer set_preamble(std::string const& name);

		private:
			/**
			 * symbol source
			 */
			symbol_reader producer_;
			symbol const* look_ahead_;
			const int verbose_;

			/**
			 * function library
			 */
			function_map	library_;

			/**
			 * generated program
			 */
			vector_t	prg_;

		};


	}	//	docscript
}	//	cyng

#endif	//	CYNG_DOCSCRIPT_COMPILER_H




