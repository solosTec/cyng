/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 


#include <cyng/docscript/compiler.h>
#include <cyng/vm/generator.h>
#include <cyng/factory.h>

namespace cyng	
{
	namespace docscript
	{

		compiler::compiler(std::list<symbol> const& sl, int verbose)
			: producer_(sl)
			, look_ahead_(&producer_.get())
			, verbose_(verbose)
			, library_()
			, prg_()
		{
			init_library();
		}

		void compiler::run(std::chrono::system_clock::time_point const& last_write_time
			, uintmax_t file_size
			, boost::filesystem::path const& out)
		{
			//
			//	call initial functions and open the "generate"
			//	call frame
			//
			prg_
				//	true ==> global function
				<< generate_invoke_unwinded("meta"
					, std::uint32_t(NL_)
					, param_map_factory("last-write-time", last_write_time)("file-size", file_size)())

				//	build a call frame generate function
				<< code::ESBA
				<< out
				;

			loop(0);

			prg_
				//	generate output file
				<< invoke("generate")
				<< code::REBA
				;

			if (verbose_ > 2)
			{
				std::cout
					<< "***info: "
					<< prg_.size()
					<< " ops generated"
					<< std::endl
					;
			}
		}

		bool compiler::loop(std::size_t depth)
		{
			while (true)
			{
				switch (look_ahead_->type_)
				{
				case SYM_EOF:
					if (verbose_ > 1)
					{
						std::cout
							<< "***info: EOF"
							<< std::endl
							;
					}
					return true;

				case SYM_FUN_NL:
					//	functions at beginning of line are global
					if (look_ahead_->value_.compare("end") == 0) {
						match(SYM_FUN_NL);
						match(SYM_FUN_CLOSE);
						return false;
					}
					else {
						fun_nl(look_ahead_->value_, depth);
					}
					break;
				case SYM_FUN_WS:
					// all other functions are local	
					fun_ws(look_ahead_->value_, depth);
					break;
				case SYM_FUN_PAR:
					//	new paragraph
					fun_par(depth);
					//fun_par(set_preamble(look_ahead_->value_));
					break;
				default:
					std::cerr
						<< "***warning: unknown symbol "
						<< (*look_ahead_)
						<< std::endl
						;
					match(look_ahead_->type_);
					break;
				}
			}

			//	unreachable
			return false;
		}


		bool compiler::match(symbol_type st)
		{
			if (look_ahead_->type_ != st)
			{
				std::cerr
					<< "***error (wrong lookahead) expected: ["
					<< name(st)
					<< "] but get "
					<< *look_ahead_
					<< std::endl
					;

				//
				//	try to recover and skip unmatching symbols
				//
				while (look_ahead_->type_ != st && look_ahead_->type_ != SYM_EOF)	{
					look_ahead_ = &producer_.get();
				}
				return false;
			}

			BOOST_ASSERT_MSG(look_ahead_->type_ == st, "wrong symbol");
			look_ahead_ = &producer_.get();

			if (verbose_ > 4)
			{
				std::cout
					<< "***info: look ahead "
					<< *look_ahead_
					<< std::endl
					;
			}

			return true;
		}

		void compiler::fun_nl(std::string name, std::size_t depth)
		{
			match(SYM_FUN_NL);
			switch (look_ahead_->type_)
			{
			case SYM_KEY:
				key(name, true, look_ahead_->value_, depth);
				break;

			case SYM_ARG:
			case SYM_FUN_WS:
				//	NL function
				arg(name, /*look_ahead_->value_, */depth);
				break;
			//case SYM_FUN_WS:
			//{
			//	//
			//	//	a local function produces input for global function
			//	//
			//	call_frame tr(lookup(name), *this, depth);
			//	fun_ws(look_ahead_->value_, depth);
			//	match(SYM_FUN_CLOSE);
			//}
			//	break;

			case SYM_FUN_CLOSE:
				//	no arguments
			{
				std::cout
					<< "***info: compile function "
					<< name
					<< "() - without arguments"
					<< std::endl
					;
				call_frame tr(lookup(name), *this, depth);
			}
			match(SYM_FUN_CLOSE);
			break;

			default:
				std::cerr
					<< "***warning: unexpected symbol (fun-nl)"
					<< (*look_ahead_)
					<< std::endl
					;
				break;
			}
		}

		void compiler::fun_ws(std::string name, std::size_t depth)
		{
			match(SYM_FUN_WS);
			switch (look_ahead_->type_)
			{
			case SYM_KEY:
				key(name, false, look_ahead_->value_, depth);
				break;
			case SYM_ARG:
				//	WS function
				arg(name, /*look_ahead_->value_, */depth);
				break;
			case SYM_FUN_WS:
			{
				call_frame tr(lookup(name), *this, depth);
				fun_ws(look_ahead_->value_, depth);

				//
				//	It's possible to get one more function call (SYM_FUN_WS) that produces the argument(s)
				//	for the previous function call. To handle this properly the order of function calls
				//	must be changed. To make the right decision, we have to know if there are missing arguments
				//	open.
				//
				//if (look_ahead_ == SYM_FUN_WS)	...
				match(SYM_FUN_CLOSE);
			}
				break;
			case SYM_FUN_CLOSE:
				//	no arguments
				{
					if (verbose_ > 3)
					{
						std::cout
							<< "***info: compile function "
							<< name
							<< "() - without arguments"
							<< std::endl
							;
					}

					//
					//	use RAII to create a function call frame
					//
					call_frame tr(lookup(name), *this, depth);
					match(SYM_FUN_CLOSE);
				}
				break;
			default:
				std::cerr
					<< "***warning: unknown symbol (fun-nl)"
					<< (*look_ahead_)
					<< std::endl
					;
				break;
			}
		}

		void compiler::fun_par(std::size_t depth)
		{
			//
			//	create call frame
			//
			call_frame tr(lookup(look_ahead_->value_), *this, depth);

			//
			//	match look ahead symbol
			//
			match(SYM_FUN_PAR);

			//
			//	Store program position to patch paragraph size at the end.
			//
			const auto prg_pos = prg_.size();
			prg_ << make_object();

			//
			//	symbol counter
			//
			std::size_t counter{ 0 };

			while ((look_ahead_->type_ != SYM_FUN_PAR) && (look_ahead_->type_ != SYM_FUN_NL) && (look_ahead_->type_ != SYM_EOF))
			{
				switch (look_ahead_->type_)
				{
				case SYM_WORD:
				case SYM_CHAR:
					prg_ << make_object(look_ahead_->value_);
					match(look_ahead_->type_);
					counter++;
					break;
				case SYM_FUN_WS:
					//	number of return values
					counter += lookup(look_ahead_->value_)->rvs_;
					fun_ws(look_ahead_->value_, 0u);
					break;
				default:
					match(look_ahead_->type_);
					counter++;
					break;
				}
			}
			if (verbose_ > 3)
			{
				std::cout
					<< "***info: compile paragraph with "
					<< counter
					<< " elements"
					<< std::endl
					;
			}

			//
			//	patch paragraph size as first parameter
			//
			prg_.at(prg_pos) = make_object<std::size_t>(counter);
		}

		void compiler::key(std::string name, bool nl, std::string key, std::size_t depth)
		{
			//
			//	open a function call and close it with RAII
			//
			call_frame tr(lookup(name), *this, depth);

			//
			//	iterate until SYM_FUN_CLOSE
			//
			std::size_t counter{ 0 };
			while (SYM_KEY == look_ahead_->type_)
			{
				match(SYM_KEY);

				switch (look_ahead_->type_)
				{
				case SYM_VALUE:
					if (verbose_ > 3)
					{
						std::cout
							<< "***info: ["
							<< name
							<< "] assemble param "
							<< key
							<< " = \""
							<< look_ahead_->value_
							<< "\""
							<< std::endl
							;
					}
					prg_
						<< make_object(look_ahead_->value_)	//	value
						<< make_object(key)	//	key
						<< code::ASSEMBLE_PARAM
						;
					match(SYM_VALUE);
					break;

				case SYM_NUMBER:
					if (verbose_ > 3)
					{
						std::cout
							<< "***info: ["
							<< name
							<< "] assemble param "
							<< key
							<< " = "
							<< std::stoull(look_ahead_->value_)
							<< std::endl
							;
					}
					prg_
						<< make_object(std::stoull(look_ahead_->value_))	//	value (string => int)
						<< make_object(key)	//	key
						<< code::ASSEMBLE_PARAM
						;
					match(SYM_NUMBER);
					break;
				case SYM_FUN_WS:
					if (verbose_ > 3)
					{
						std::cout
							<< "***info: compile function "
							<< name
							<< " with "
							<< key
							<< " = "
							<< look_ahead_->value_
							<< "(...)"
							<< std::endl
							;
					}
					fun_ws(look_ahead_->value_, depth);
					prg_
						<< make_object(key)	//	key
						<< code::ASSEMBLE_PARAM
						;
					break;
				default:
					//	error
					break;
				}

				//
				//	next key
				//
				key = look_ahead_->value_;
				counter++;
			}

			//
			//	function call complete
			//
			match(SYM_FUN_CLOSE);

			//
			//	provide basic function call parameters
			//	* key:value pairs
			//	* NL/WS function
			//
			prg_
				<< make_object(counter)	//	parameters
				<< code::ASSEMBLE_PARAM_MAP

				//
				//	close call frame and call function
				//
				<< make_object(depth)
				;

			//
			//	test for environment
			//
			if (tr.fp_->type_ == ENV_RAW) {

				//
				//	skip pilcrow
				//
				if (SYM_FUN_PAR == look_ahead_->type_)	match(SYM_FUN_PAR);

				//
				//	provide all symbols unprocessed until .end()
				//
				while (look_ahead_->type_ != SYM_EOF) {
					if ((SYM_FUN_NL == look_ahead_->type_) && (look_ahead_->value_.compare("end") == 0)) {
						match(SYM_FUN_NL);
						match(SYM_FUN_CLOSE);
						break;
					}
					else {
						prg_ << make_object(look_ahead_->value_);
						match(look_ahead_->type_);
					}
				}

			}
			else if (tr.fp_->type_ == ENV_PROCESSED) {

				//
				//	full formatting supported
				//
				loop(++depth);
			}


			//
			//	implicit create REBA op with desctructor of trailer
			//
		}

		std::size_t compiler::arg(std::string name, /*std::string value, */std::size_t depth)
		{
			//
			//	call frame established
			//
			call_frame tr(lookup(name), *this, depth);

			//
			//	collect all arguments by iterating until SYM_FUN_CLOSE
			//
			std::size_t counter{ 0 };
			while (look_ahead_->type_ != SYM_FUN_CLOSE && look_ahead_->type_ != SYM_EOF)
			{
				//
				// process argument (value)
				//
				if (verbose_ > 3)
				{
					std::cout
						<< "***info: compile function "
						<< name
						<< " argument #"
						<< counter
						<< ": "
						<< look_ahead_->value_
						<< std::endl
						;
				}

				
				if (SYM_FUN_WS == look_ahead_->type_)
				{
					//
					//	a local function produces an argument
					//
					fun_ws(look_ahead_->value_, depth);
				}
				else
				{
					//
					//	produce arguments
					//
					prg_ << make_object(look_ahead_->value_);
					match(SYM_ARG);
					//value = look_ahead_->value_;
				}
				counter++;
			}

			match(SYM_FUN_CLOSE);
			return counter;
		}

		void compiler::init_library()
		{
			//
			//	Substitute short cuts with full function name.
			//	example: "em", "italic" and "i" are all the same and translate 
			//	to "emphasise".
			//
			insert(library_, std::make_shared<function>("bold", 1, WS_), { "b" });
			insert(library_, std::make_shared<function>("emphasise", 1, WS_), { "i", "italic", "em" });
			insert(library_, std::make_shared<function>("color", 1, WS_), { "col" });
			insert(library_, std::make_shared<function>("reference", 1, WS_), { "ref" });
			insert(library_, std::make_shared<function>("figure", 1, NL_), { "fig" });

			insert(library_, std::make_shared<function>("meta", 0, NL_), {});
			insert(library_, std::make_shared<function>("title", 0, NL_), {});
			insert(library_, std::make_shared<function>("debug", 0, NL_), {});
			insert(library_, std::make_shared<function>("contents", 0, NL_), {});
			insert(library_, std::make_shared<function>("item", 1, NL_), { "entry" });

			insert(library_, std::make_shared<function>("paragraph", 1, NL_), { u8"¶" });
			insert(library_, std::make_shared<function>("header", 1, NL_), { "h" });
			insert(library_, std::make_shared<function>("header.1", 1, NL_), { "h1" });
			insert(library_, std::make_shared<function>("header.2", 1, NL_), { "h2" });
			insert(library_, std::make_shared<function>("header.3", 1, NL_), { "h3" });
			insert(library_, std::make_shared<function>("header.4", 1, NL_), { "h4" });

			insert(library_, std::make_shared<function>("quote", 1, ENV_RAW), { "q" });
			insert(library_, std::make_shared<function>("cite", 1, ENV_PROCESSED), { "c" });
			insert(library_, std::make_shared<function>("list", 1, ENV_PROCESSED), { "l" });
			insert(library_, std::make_shared<function>("env.open", 1, ENV_RAW), { "+" });
			insert(library_, std::make_shared<function>("env.close", 1, ENV_RAW), { "-" });
		}

		compiler::fp compiler::lookup(std::string const& name) const
		{
			return cyng::docscript::lookup(library_, name);
		}

		vector_t move_program(compiler& c)
		{
			return std::move(c.prg_);
		}


		compiler::call_frame::call_frame(compiler::fp p, compiler& c, std::size_t depth)
			: fp_(p)
			, compiler_(c)
			, depth_(depth)
			, pos_(c.prg_.size())
		{
			BOOST_ASSERT(!!p);	//	it's guaranteed to get a valid pointer
			for (auto idx = decltype(p->rvs_){0}; idx < p->rvs_; ++idx)
			{
				compiler_.prg_ << code::ASP;	//	return value(s)
			}

			compiler_.prg_
				<< code::ESBA
				<< static_cast<std::uint32_t>(p->type_)	//	function type
				;

		}

		compiler::call_frame::call_frame(call_frame&& tr)
			: fp_(std::move(tr.fp_))
			, compiler_(tr.compiler_)
			, depth_(tr.depth_)
			, pos_(tr.pos_)
		{}

		compiler::call_frame::~call_frame()
		{
			if (compiler_.verbose_ > 3)
			{
				std::cout
					<< "***info: generate function call "
					<< fp_->name_
					<< " with #"
					<< fp_->rvs_
					<< " return value(s)"
					<< std::endl
					;
			}

			compiler_.prg_ << invoke(fp_->name_);

			//	set return value(s)
			for (auto idx = decltype(fp_->rvs_){0}; idx < fp_->rvs_; ++idx)
			{
				compiler_.prg_
					<< (idx + 1)
					<< code::PR
					;
			}

			compiler_.prg_ << code::REBA;

		}

	}	//	docscript
}	//	cyng


