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
			param_map_t params;
			params["last-write-time"] = make_object(last_write_time);
			params["file-size"] = make_object(file_size);
			prg_
				//	true ==> global function
				<< generate_invoke_unwinded("meta", std::size_t(1), true, params)

				//	build a call frame generate function
				<< code::ESBA
				<< out
				;

			bool loop = true;
			while (loop)
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
					loop = false;
					break;

				case SYM_FUN_NL:
					//	functions at beginning of line are global
					fun_nl(look_ahead_->value_);
					break;
				case SYM_FUN_WS:
					// all other functions are local	
					fun_ws(look_ahead_->value_);
					break;
				case SYM_FUN_PAR:
					//	new paragraph
					fun_par(set_preamble(look_ahead_->value_));
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

			prg_
				//	generate output file
				<< invoke("generate")
				<< code::REBA
				;
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
				//	skip unmatching symbols
				//
				while (look_ahead_->type_ != st && look_ahead_->type_ != SYM_EOF)
				{
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

		void compiler::fun_nl(std::string name)
		{
			match(SYM_FUN_NL);
			switch (look_ahead_->type_)
			{
			case SYM_KEY:
				key(name, true, look_ahead_->value_);
				break;
			case SYM_ARG:
				//	single argument
				arg(name, true, look_ahead_->value_);
				break;
			case SYM_FUN_WS:
			{
				//	overwrite name
				trailer tr(set_preamble(name));
				fun_ws(look_ahead_->value_);
				match(SYM_FUN_CLOSE);

				//prg_
				//	//	we asume that the function produced 1 return value
				//	<< make_object<std::size_t>(1)
				//	<< invoke(name)
				//	<< code::REBA
				//	;
			}
				break;
			case SYM_FUN_CLOSE:
				//	no arguments
			{
				std::cout
					<< "***info: compile function "
					<< name
					<< "() - without arguments"
					<< std::endl
					;
				trailer tr(set_preamble(name));
			}
			match(SYM_FUN_CLOSE);
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

		void compiler::fun_ws(std::string name)
		{
			match(SYM_FUN_WS);
			switch (look_ahead_->type_)
			{
			case SYM_KEY:
				key(name, false, look_ahead_->value_);
				break;
			case SYM_ARG:
				//	single argument
				arg(name, false, look_ahead_->value_);
				break;
			case SYM_FUN_WS:
			{
				trailer tr(set_preamble(name));
				fun_ws(look_ahead_->value_);

				//
				//	It's possible to get one more function call (SYM_FUN_WS) that produces the argument(s)
				//	for the previous function call. To handle this properly the order of function calls
				//	must be changed. To make the right decision, we have to know if there are missing arguments
				//	open.
				//
				//if (look_ahead_ == SYM_FUN_WS)	...
				match(SYM_FUN_CLOSE);

				//prg_
				//	<< make_object(false)		//	local function
				//	<< make_object(lookup(name)->rvs_)
				//	<< invoke(name)
				//	<< code::REBA
				//	;
			}
				break;
			case SYM_FUN_CLOSE:
				//	no arguments
				if (verbose_ > 3)
				{
					std::cout
						<< "***info: compile function "
						<< name
						<< "() - without arguments"
						<< std::endl
						;
				}
				{
					trailer tr(set_preamble(name));
					//prg_
					//	<< make_object<std::size_t>(0)
					//	<< invoke(name)
					//	<< code::REBA
					//	;

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

		void compiler::fun_par(trailer&& tr)
		{
			match(SYM_FUN_PAR);
			std::size_t counter{ 0 };

			while (look_ahead_->type_ != SYM_FUN_PAR && look_ahead_->type_ != SYM_FUN_NL && look_ahead_->type_ != SYM_EOF)
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
					fun_ws(look_ahead_->value_);
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
			//prg_
			//	<< make_object<std::size_t>(counter)
			//	<< invoke(name)
			//	<< code::REBA
			//	;
		}

		void compiler::key(std::string name, bool nl, std::string key)
		{
			trailer tr(set_preamble(name));

			//
			//	iterate until SYM_FUN_CLOSE
			//
			std::size_t counter{ 0 };
			while (look_ahead_->type_ != SYM_FUN_CLOSE && look_ahead_->type_ != SYM_EOF)
			{
				match(SYM_KEY);

				switch (look_ahead_->type_)
				{
				case SYM_VALUE:
					if (verbose_ > 3)
					{
						std::cout
							<< "***info: compile function "
							<< name
							<< " with "
							<< key
							<< " = "
							<< look_ahead_->value_
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
							<< "***info: compile function "
							<< name
							<< " with "
							<< key
							<< " = "
							<< look_ahead_->value_
							<< std::endl
							;
					}
					prg_
// 						<< index_from_str_factory(look_ahead_->value_)	//	value
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
					fun_ws(look_ahead_->value_);
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
			match(SYM_FUN_CLOSE);

			if (look_ahead_->type_ == SYM_ARG)
			{
				if (verbose_ > 3)
				{
					std::cout
						<< "***info: environment "
						<< name
						<< " has an argument of "
// 						<< bytes_format(look_ahead_->value_.size())
						<< std::endl
						;
				}

				counter++;
				prg_
					<< make_object(look_ahead_->value_)	//	value
					<< make_object("$")	//	key
					<< code::ASSEMBLE_PARAM
					;

				//	
				//	This is the content of an environment
				//
				match(SYM_ARG);
			}

			prg_
				<< make_object(counter)	//	parameters
				<< code::ASSEMBLE_PARAM_MAP

				//
				//	close call frame and call function
				//
				<< make_object(nl)
				//<< make_object<std::size_t>(1)	//	one parameter map
				//<< invoke(name)
				//<< code::REBA
				;

		}

		void compiler::arg(std::string name, bool nl, std::string value)
		{
			trailer tr(set_preamble(name));

			//
			//	iterate until SYM_FUN_CLOSE
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
						<< value
						<< std::endl
						;
				}

				prg_
					<< make_object(value);
				;

				match(SYM_ARG);
				value = look_ahead_->value_;
				counter++;
			}

			match(SYM_FUN_CLOSE);

			//
			//	close call frame and call function
			//
			//prg_
			//	<< make_object(nl)
			//	<< make_object(counter)
			//	<< invoke(name)
			//	<< code::REBA
			//	;

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
			insert(library_, std::make_shared<function>("figure", 1, WS_), { "fig" });

			insert(library_, std::make_shared<function>("meta", 0, NL_), {});
			insert(library_, std::make_shared<function>("title", 0, NL_), {});
			insert(library_, std::make_shared<function>("debug", 0, NL_), {});
			insert(library_, std::make_shared<function>("contents", 0, NL_), {});

			insert(library_, std::make_shared<function>("paragraph", 1, NL_), { u8"¶" });
			insert(library_, std::make_shared<function>("header", 1, NL_), { "h" });
			insert(library_, std::make_shared<function>("header.1", 1, NL_), { "h1" });
			insert(library_, std::make_shared<function>("header.2", 1, NL_), { "h2" });
			insert(library_, std::make_shared<function>("header.3", 1, NL_), { "h3" });
			insert(library_, std::make_shared<function>("header.4", 1, NL_), { "h4" });

			insert(library_, std::make_shared<function>("quote", 1, ENV_), { "q" });
			insert(library_, std::make_shared<function>("env.open", 1, ENV_), { "+" });
			insert(library_, std::make_shared<function>("env.close", 1, ENV_), { "-" });
		}

		compiler::fp compiler::lookup(std::string const& name) const
		{
			return cyng::docscript::lookup(library_, name);
		}

		compiler::trailer compiler::set_preamble(std::string const& name)
		{
			auto fp = lookup(name);
			BOOST_ASSERT(!!fp);	//	it's guaranteed to get a valid pointer
			for (auto idx = decltype(fp->rvs_){0}; idx < fp->rvs_; ++idx)
			{
				prg_ << code::ASP;	//	return value(s)
			}
			prg_ 
				<< code::ESBA
				<< static_cast<std::uint32_t>(fp->type_)	//	function type
				//<< fp->rvs_	//	return value count
				;
			return trailer(verbose_, prg_, fp);
		}

		vector_t move_program(compiler& c)
		{
			return std::move(c.prg_);
		}


		compiler::trailer::trailer(int verbose, vector_t& prg, fp f)
			: fp_(f)
			, verbose_(verbose)
			, prg_(prg)
		{}

		compiler::trailer::trailer(trailer&& tr)
			: fp_(std::move(tr.fp_))
			, verbose_(tr.verbose_)
			, prg_(std::move(tr.prg_))
		{}

		compiler::trailer::~trailer()
		{
			if (verbose_ > 3)
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

			prg_
				//<< fp_->rvs_	//	return value count
				<< invoke(fp_->name_)
				;

			//	set return value(s)
			for (auto idx = decltype(fp_->rvs_){0}; idx < fp_->rvs_; ++idx)
			{
				prg_
					<< (idx + 1)
					<< code::PR
					;
			}

			prg_ << code::REBA;

		}

	}	//	docscript
}	//	cyng


