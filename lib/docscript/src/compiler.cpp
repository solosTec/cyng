/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 


#include <cyng/docscript/compiler.h>
#include <cyng/vm/generator.h>
#include <cyng/factory.h>
#include <boost/algorithm/string.hpp>

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
			, line_(0)
			, column_(0)
			, source_file_()
		{
			init_library();
		}

		void compiler::run(boost::filesystem::path out, bool meta)
		{
			//
			//	call initial functions and open the "generate"
			//	call frame
			//
			init();

			//
			//	build a call frame generate function
			//
			prg_
				<< code::ESBA
				<< out
				;

			//
			//	main loop until EOF
			//
			loop(0);

			//
			//	generate output file
			//
			prg_
				<< invoke("generate.file")
				<< code::REBA
				;

			if (meta) {

				//
				//	generate meta 
				//
				out.replace_extension(".json");
				prg_ << generate_invoke_unwinded("generate.meta", out);
			}

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

		void compiler::init()
		{
			//
			//	use RAII to create a function call frame
			//
			call_frame cf(lookup("meta"), *this, 0, PARAMETERS);
			prg_ << meta_;
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
						func(look_ahead_->value_, depth, true);
					}
					break;
				case SYM_FUN_WS:
					// all other functions are local	
					func(look_ahead_->value_, depth, false);
					break;
				case SYM_FUN_PAR:
					//	new paragraph
					fun_par(depth);
					break;

				case SYM_FUN_ENV:
					//	new environment
					fun_env(depth);
					break;

				default:
					std::cerr
						<< "***warning: unknown symbol "
						<< (*look_ahead_)
						<< " #"
						<< line_
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
					<< " #"
					<< line_
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

			do {

				//
				//	next symbol
				//
				look_ahead_ = &producer_.get();

				switch (look_ahead_->type_)
				{
					case SYM_LINE:
						line_ = look_ahead_->value_.at(0);
						if (verbose_ > 5)
						{
							std::cout
								<< "***info: update source line #"
								<< line_
								<< std::endl
								;
						}
						look_ahead_ = &producer_.get();
						break;
					case SYM_COL:
						look_ahead_ = &producer_.get();
						break;
					case SYM_FILE:
						look_ahead_ = &producer_.get();
						break;
				}

			} while (look_ahead_->type_ == SYM_LINE
				|| look_ahead_->type_ == SYM_COL
				|| look_ahead_->type_ == SYM_FILE);


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

		void compiler::func(std::string name, std::size_t depth, bool nl)
		{
			//
			//	match and produce next look ahead
			//
			match(nl ? SYM_FUN_NL : SYM_FUN_WS);

			switch (look_ahead_->type_)
			{
			case SYM_KEY:
				key(name, true, look_ahead_->value_, depth);
				break;

			case SYM_VALUE:
			case SYM_FUN_WS:
				//	NL function
				arg(name, depth);
				break;

			case SYM_FUN_CLOSE:
				//	no arguments
			{
				std::cout
					<< "***info: compile function ["
					<< name
					<< "]() - without arguments"
					<< std::endl
					;
				call_frame tr(lookup(name), *this, depth, PARAMETERS);
			}
			match(SYM_FUN_CLOSE);
			break;

			default:
				std::cerr
					<< "***warning: unexpected symbol (fun-nl)"
					<< (*look_ahead_)
					<< " #"
					<< line_
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
			call_frame tr(lookup(look_ahead_->value_), *this, depth, ARGUMENT_LIST);	//	not a key function

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

			bool stop = false;
			while (!stop)
			{
				switch (look_ahead_->type_)
				{
				case SYM_CHAR:
					BOOST_ASSERT(look_ahead_->value_.size() == 1);
					switch (look_ahead_->value_.front()) {
					case '<':
						prg_ << make_object("&lt;");
						match(look_ahead_->type_);
						counter++;
						continue;
					case '>':
						prg_ << make_object("&gt;");
						match(look_ahead_->type_);
						counter++;
						continue;
					case '&':
						prg_ << make_object("&amp;");
						match(look_ahead_->type_);
						counter++;
						continue;
					case '"':
						prg_ << make_object("&quot;");
						match(look_ahead_->type_);
						counter++;
						continue;
					case '\'':
						prg_ << make_object("&apos;");
						match(look_ahead_->type_);
						counter++;
						continue;
					default:
						break;
					}

					//
					//	fall through - handle SYM_CHAR as SYM_WORD
					//
				case SYM_WORD:
					emit();
					counter++;
					break;
				case SYM_FUN_NL:
					if (!lookup(look_ahead_->value_)->is_ws()) {
						stop = true;
						break;
					}
					counter += lookup(look_ahead_->value_)->rvs_;
					func(look_ahead_->value_, depth, true);
					break;
				case SYM_FUN_WS:
					//	number of return values
					counter += lookup(look_ahead_->value_)->rvs_;
					func(look_ahead_->value_, depth, false);
					break;
				case SYM_FUN_PAR:
				case SYM_FUN_ENV:
				case SYM_EOF:
					stop = true;
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

		void compiler::fun_env(std::size_t depth)
		{
			auto const p = lookup(look_ahead_->value_);
			match(SYM_FUN_ENV);
			key(p->name_, true, look_ahead_->value_, depth);
		}

		void compiler::key(std::string name, bool nl, std::string key, std::size_t depth)
		{
			//
			//	open a function call and close it with RAII
			//
			call_frame tr(lookup(name), *this, depth, PARAMETERS);	//	function with key/value params

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
							<< '.'
							<< key
							<< "] = \""
							<< look_ahead_->value_
							<< "\""
							<< std::endl
							;
					}

					prg_
						<< produce_value(look_ahead_->value_)
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
							<< '.'
							<< key
							<< "] = "
							<< std::stoull(look_ahead_->value_)
							<< std::endl
							;
					}

					prg_
						<< produce_number(look_ahead_->value_)
						<< make_object(key)	//	key
						<< code::ASSEMBLE_PARAM
						;
					match(SYM_NUMBER);
					break;

				case SYM_FUN_WS:
					if (verbose_ > 3)
					{
						std::cout
							<< "***info: compile function ["
							<< name
							<< '.'
							<< key
							<< "] = "
							<< look_ahead_->value_
							<< "(...)"
							<< std::endl
							;
					}
					func(look_ahead_->value_, depth, false);
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
				;

			//
			//	test for environment
			//
			switch (tr.fp_->type_) {
			case ENV_RAW:
				//
				//	skip pilcrow
				//
				if (SYM_FUN_PAR == look_ahead_->type_)	match(SYM_FUN_PAR);
				env_raw();
				break;
			case ENV_PROCESSED:
				//
				//	full formatting supported
				//
				loop(depth + 1);
				break;
			case ENV_DSL:
				//
				//	skip pilcrow
				//
				if (SYM_FUN_PAR == look_ahead_->type_)	match(SYM_FUN_PAR);
				env_dsl();
				break;
			default:
				break;
			}

			//
			//	implicit create REBA op with desctructor of trailer
			//
		}

		void compiler::env_raw()
		{
			//
			//	provide all symbols unprocessed until .end()
			//
			while (SYM_EOF != look_ahead_->type_) {
				if ((SYM_FUN_NL == look_ahead_->type_) && (look_ahead_->value_.compare("end") == 0)) {
					match(SYM_FUN_NL);
					match(SYM_FUN_CLOSE);
					break;
				}
				else {
					emit();
				}
			}
		}

		void compiler::env_dsl()
		{
			while (SYM_EOF != look_ahead_->type_) {
				if ((SYM_FUN_ENV == look_ahead_->type_)) {
					auto const p = lookup(look_ahead_->value_);
					if (p->name_.compare("env.close") != 0) {
						std::cerr
							<< "***error: end of environment expected "
							<< (*look_ahead_)
							<< " #"
							<< line_
							<< std::endl
							;
					}
					match(SYM_FUN_ENV);
					if (SYM_FUN_CLOSE != look_ahead_->type_) {
						std::cerr
							<< "***error: no parameters allowed at end of environment"
							<< (*look_ahead_)
							<< " #"
							<< line_
							<< std::endl
							;
					}
					match(SYM_FUN_CLOSE);
					break;
				}
				else {
					emit();
				}
			}
		}

		std::size_t compiler::arg(std::string name, std::size_t depth)
		{
			//
			//	call frame established
			//
			call_frame tr(lookup(name), *this, depth, ARGUMENT_LIST);	//	function with argument list

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
						<< "***info: compile function ["
						<< name
						<< ".#"
						<< counter
						<< "] = "
						<< look_ahead_->value_
						<< std::endl
						;
				}

				
				switch (look_ahead_->type_) {
				case SYM_FUN_WS:
					//
					//	a local function produces an argument
					//
					func(look_ahead_->value_, depth + 1, false);
					break;
				case SYM_NUMBER:
					if (!look_ahead_->value_.empty())	prg_ << produce_number(look_ahead_->value_);
					match(SYM_NUMBER);
					break;
				default:
					//
					//	produce arguments
					//
					if (!look_ahead_->value_.empty())	prg_ << produce_value(look_ahead_->value_);
					match(SYM_VALUE);
					break;
				}

				counter++;
			}

			match(SYM_FUN_CLOSE);
			return counter;
		}

		void compiler::emit()
		{
			if (!look_ahead_->value_.empty())	prg_ << make_object(look_ahead_->value_);
			match(look_ahead_->type_);
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
			insert(library_, std::make_shared<function>("contents", 1, NL_), {});
			insert(library_, std::make_shared<function>("item", 1, NL_), { "entry" });


			insert(library_, std::make_shared<function>("paragraph", 1, NL_), {  });
//			insert(library_, std::make_shared<function>("paragraph", 1, NL_), { u8"¶" });
			insert(library_, std::make_shared<function>("header", 1, NL_), { "h" });
			insert(library_, std::make_shared<function>("header.1", 1, NL_), { "h1" });
			insert(library_, std::make_shared<function>("header.2", 1, NL_), { "h2" });
			insert(library_, std::make_shared<function>("header.3", 1, NL_), { "h3" });
			insert(library_, std::make_shared<function>("header.4", 1, NL_), { "h4" });

			insert(library_, std::make_shared<function>("quote", 1, ENV_RAW), { "q" });
			insert(library_, std::make_shared<function>("cite", 1, ENV_PROCESSED), { "c" });
			insert(library_, std::make_shared<function>("list", 1, ENV_PROCESSED), { "l" });
			insert(library_, std::make_shared<function>("env.open", 1, ENV_DSL), { "+", "start" });
			insert(library_, std::make_shared<function>("env.close", 1, ENV_DSL), { "-", "end" });
			insert(library_, std::make_shared<function>("end", 0, NL_), {});
		}

		compiler::fp compiler::lookup(std::string const& name) const
		{
			return cyng::docscript::lookup(library_, name);
		}

		vector_t move_program(compiler& c)
		{
			return std::move(c.prg_);
		}


		compiler::call_frame::call_frame(compiler::fp p, compiler& c, std::size_t depth, call_frame_type cft)
			: fp_(p)
			, compiler_(c)
			, depth_(depth)
			, pos_(c.prg_.size())
			, has_params_(PARAMETERS == cft)
		{
			BOOST_ASSERT(!!p);	//	it's guaranteed to get a valid pointer
			for (auto idx = decltype(p->rvs_){0}; idx < p->rvs_; ++idx)
			{
				compiler_.prg_ << code::ASP;	//	return value(s)
			}

			compiler_.prg_
				<< code::ESBA
				<< static_cast<std::uint16_t>(p->type_)	//	function type
				<< depth
				<< has_params_
				;

		}

		compiler::call_frame::call_frame(call_frame&& tr)
			: fp_(std::move(tr.fp_))
			, compiler_(tr.compiler_)
			, depth_(tr.depth_)
			, pos_(tr.pos_)
			, has_params_(tr.has_params_)
		{}

		compiler::call_frame::~call_frame()
		{
			if (compiler_.verbose_ > 3)
			{
				std::cout
					<< "***info: generate function ["
					<< fp_->rvs_
					<< "] "
					<< fp_->name_
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

		cyng::object produce_value(std::string val)
		{
			if (boost::algorithm::equals("true", val)) {
				return cyng::make_object(true);
			}
			else if (boost::algorithm::equals("false", val)) {
				return cyng::make_object(false);
			}
			return cyng::make_object(val);
		}

		cyng::object produce_number(std::string val)
		{
			try {
				return cyng::make_object(std::stoull(val));
			}
			catch (std::invalid_argument const&) {
				return cyng::make_object(val);
			}
			catch (std::out_of_range const&) {
				return cyng::make_object(0);
			}
		}

	}	//	docscript
}	//	cyng


