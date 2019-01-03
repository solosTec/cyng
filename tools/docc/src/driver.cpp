/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 


#include "driver.h"
#include "reader.h"
#include <cyng/docscript/compiler.h>
#include <cyng/docscript/generator.h>
#include <cyng/io/serializer.h>
#include <cyng/factory.h>
#include <cyng/io/parser/parser.h>
#include <cyng/io/io_chrono.hpp>

#include <iostream>

namespace cyng
{
	namespace docscript
	{
	
		driver::driver(std::vector< std::string >const& inc
			, int verbose)
		: includes_(inc.begin(), inc.end())
			, verbose_(verbose)
			, last_write_time_(std::chrono::system_clock::now())
			, file_size_(0)
			, stats_()
			, parentheses_(0)
			, buffer_()
			, tokenizer_([this](docscript::token&& tok) {

			if (!tok.eof_)
			{
				//	update frequency
				stats_[tok.value_]++;

				//
				//	build up symbols
				//
				for (auto idx = decltype(tok.count_){0}; idx < tok.count_; ++idx)
				{
					if (verbose_ > 4)
					{
						std::cout
							<< "***info: TOKEN "
							<< tok
							<< " - state "
							<< lexer_.get_state()
							<< " - "
							<< lexer_.dump_stack()
							<< " #"
							<< line_
							;

						if (!source_files_.empty()) {
							std::cout
								<< '@'
								<< source_files_.top()
								;
						}
						std::cout
							<< std::endl
							;
					}
					while (!lexer_.next(tok.value_))
					{
						if (verbose_ > 4)
						{
							std::cout
								<< "***info: repeat"
								<< tok
								<< std::endl;
						}
					}
				}
			}
			else {

				//
				//	eof
				//
				lexer_.next(tok.value_);
			}
		})
		, lexer_([this](symbol&& sym) {

			if (verbose_ > 3)
			{ 
				std::cout 
				<< "SYMBOL  " 
				<< sym 
				<< std::endl;
			}

			switch (sym.type_)
			{
			case SYM_FUN_NL:		
				//	functions at beginning of line are global
			case SYM_FUN_WS:		
				//	all other functions are local	
				parentheses_++;
				break;
			case SYM_FUN_CLOSE:
				//	all function arguments read
				parentheses_--;
				break;
			case SYM_EOF:
				if (parentheses_ != 0) {
					std::cerr
						<< "***error: unbalanced parentheses #"
						<< line_
						<< '@'
						<< source_files_.top()
						<< std::endl
						;
				}
				break;
			default:
				break;
			}

			if (parentheses_ < 0)
			{
				std::cerr
					<< "***error: unbalanced parentheses #"
					<< line_
					<< '@'
					<< source_files_.top()
					<< std::endl
					;
			}


			buffer_.emplace_back(std::move(sym));

		})
		, line_(0u)
		, source_files_()

		{}
		
		driver::~driver()
		{}

		int driver::run(boost::filesystem::path const& master
			, boost::filesystem::path const& body
			, boost::filesystem::path const& out
			, bool body_only
			, bool meta)
		{
			//
			//	get a timestamp to measure performance
			//
			const auto now = std::chrono::system_clock::now();

			//
			//	read and tokenize file recursive
			//
			const int r = open_and_run(master, 0);
			finish(body, out, meta);

			//
			//	calculate duration of reading and compilation
			//
			std::chrono::milliseconds delta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - now);
			if (verbose_ > 0)
			{
				std::cout
					<< "***info: compilation took "
					<< to_str(delta)
					<< std::endl
					;
			}

			if (!out.empty())
			{
				std::cout
					<< "***info: output file is "
					<< out
					<< std::endl
					;
				process(body, out, body_only, delta);
			}
			else
			{
				std::cerr
					<< "***warning: no output file specified"
					<< std::endl
					;
			}
			
			return r;
		}

		int driver::run(boost::filesystem::path const& inp, std::size_t depth)
		{	
			reader r(*this, inp);
			return r.run(depth)
				? EXIT_SUCCESS
				: EXIT_FAILURE
				;
		}

		int driver::open_and_run(boost::filesystem::path const& inp, std::size_t depth)
		{
			BOOST_ASSERT_MSG(depth < 64, "maximal nesting depth exceeded");
			const auto p = verify_extension(inp, "docscript");

			//
			//	open file and read it line by line
			//
			for (auto dir : includes_)
			{
				if (boost::filesystem::exists(dir / p))
				{
					if (depth == 0)
					{
						//
						//	get meta data of master file
						//
						std::tie(last_write_time_, file_size_) = read_meta_data(dir / p);
					}
					return run(dir / p, depth);
				}
			}

			std::cerr
				<< "***error: "
				<< p
				<< " not found"
				<< std::endl
				;

			return EXIT_FAILURE;

		}

		void driver::finish(boost::filesystem::path const& body, boost::filesystem::path const& out, bool meta)
		{
			//
			//	test lexer state
			//
			if (!lexer_.complete())
			{
				if (verbose_ > 1)
				{
					//
					//	invalid lexer state
					//
					std::cerr
						<< "***warning: invalid lexer state"
						<< std::endl
						;
				}
			}

			//
			//	create intermediate output file for compiler
			//
			std::ofstream file(body.string(), std::ios::out | std::ios::trunc | std::ios::binary);
			if (!file.is_open())
			{
				std::cerr
					<< "***error connot open file "
					<< body
					<< std::endl;

			}
			else 
			{ 

				if (verbose_ > 0)
				{
					std::cout
						<< "***info: start compiler with "
						<< buffer_.size()
						<< " input symbols"
						<< std::endl
						;
				}

				//	1. and 2. pass complete
				//	start compiler
				cyng::vector_t doc;
				doc.reserve(buffer_.size());	//	that's an estimation

				docscript::compiler c(buffer_, verbose_);
				c.meta_["last-write-time"] = make_object(last_write_time_);
				c.meta_["file-size"] = make_object(file_size_);
				c.meta_["file-name"] = make_object(out.filename().string());

				if (verbose_ > 1)
				{
					//
					//	automatic generated meta data
					//
					std::cout
						<< "***info: last write time: "
						<< cyng::io::to_str(cyng::make_object(last_write_time_))
						<< std::endl
						;

					std::cout
						<< "***info: file size of master file: "
						<< file_size_
						<< " bytes"
						<< std::endl
						;

					//
					//	calculated entropy
					//
					auto const entropy = calculate_entropy(stats_);
					auto const size = calculate_size(stats_);	//	symbol count

					std::cout
						<< "***info: entropy is "
						<< entropy
						<< " (calculated over "
						<< size
						<< " input token)"
						<< std::endl
						;

					c.meta_["text-entropy"] = make_object(entropy);
					c.meta_["input-symbols"] = make_object(size);

				}

				//
				//	start compiler
				//
				c.run(out, meta);


				//
				//	get program out of compiler
				//
				cyng::vector_t prg = docscript::move_program(c);
				std::cout
					<< "***info: write "
					<< prg.size()
					<< " ops to "
					<< body
					<< std::endl
					;


#ifdef _DEBUG
				for (auto obj : prg)
				{
					std::cout
						<< cyng::io::to_str(obj)
						<< ((obj.get_class().tag() == TC_CODE) ? "\n" : " ")
						;
				}
#endif
				//
				//	serialize as program not as data (reverse on stack)
				//
				std::for_each(prg.begin(), prg.end(), [&file](cyng::object const& obj) {
					cyng::io::serialize_binary(file, obj);
				});
			}

		}

		void driver::process(boost::filesystem::path const& in
			, boost::filesystem::path out
			, bool body_only
			, std::chrono::milliseconds compile_time)
		{
			//
			//	read intermdiate file
			//
			std::ifstream file(in.string(), std::ios::binary);
			if (!file.is_open())
			{
				std::cerr
					<< "***error connot open input file "
					<< in
					<< std::endl;
			}
			else
			{
				//
				//	do not skip 
				//
				file.unsetf(std::ios::skipws);

				//
				//	parse temporary file
				//
				cyng::vector_t prg;
				cyng::parser np([&prg](cyng::vector_t&& vec) {
					prg = std::move(vec);
				});
				np.read(std::istream_iterator<char>(file), std::istream_iterator<char>());

				//
				//	startup VM/generator
				//
				docscript::generator gen(this->includes_, this->verbose_, body_only);
				gen.run(prg, compile_time);
// 				if (ec)
// 				{
// 					std::cout 
// 						<< "***error: generator stopped with message "
// 						<< ec.message()
// 						<< std::endl;
// 				}

				//
				//	generate index 
				//
				//gen.index(out);

			}
		}

		std::tuple<std::chrono::system_clock::time_point, uintmax_t> read_meta_data(boost::filesystem::path p)
		{
			return (boost::filesystem::exists(p) && boost::filesystem::is_regular(p))
				? std::make_tuple(std::chrono::system_clock::from_time_t(boost::filesystem::last_write_time(p)), boost::filesystem::file_size(p))
				: std::make_tuple(std::chrono::system_clock::now(), uintmax_t(0u))
				;
		}

		boost::filesystem::path verify_extension(boost::filesystem::path p, std::string const& ext)
		{
			if (!p.has_extension())
			{
				p.replace_extension(ext);
			}
			return p;	
		}
	}	
}
