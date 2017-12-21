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
			//, doc_()
			, tokenizer_([this](docscript::token&& tok) {

				//if (verbose_ > 4)
				//{
				//	std::cout
				//		<< "***info: TOKEN "
				//		<< tok
				//		<< std::endl;
				//}

				//	update frequency
				stats_[tok.value_]++;

				//
				//	build up symbols
				//
				for (auto idx = 0; idx < tok.count_; ++idx)
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
							<< std::endl;
					}
					while(!lexer_.next(tok.value_))
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

			})
			, lexer_([this](symbol&& sym) {

				if (verbose_ > 3)
				{ 
					std::cout 
					<< "SYMBOL " 
					//<< std::setw(4)
					//<< buffer_.size()
					<< ' '
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
				default:
					break;
				}

				if (parentheses_ < 0)
				{
					std::cerr
						<< "***error: unbalanced parentheses"
						<< std::endl
						;

				}
				buffer_.emplace_back(std::move(sym));

			})

		{}
		
		driver::~driver()
		{}

		int driver::run(boost::filesystem::path const& master
			, boost::filesystem::path const& body
			, boost::filesystem::path const& out
			, bool body_only)
		{
			//
			//	get a timestamp to measure performance
			//
			const auto now = std::chrono::system_clock::now();

			//
			//	read and tokenize file recursive
			//
			const int r = open_and_run(master, 0);
			finish(body, out);

			//
			//	calculate duration of reading and compilation
			//
			auto delta = std::chrono::system_clock::now() - now;
			if (verbose_ > 0)
			{
				std::cout
					<< "***info: compilation took "
					<< cyng::io::to_str(cyng::make_object(delta))
// 					<< cyng::timespan_format(delta)
					<< std::endl
					;
			}

			//buffer_.clear();
			if (!out.empty())
			{
				std::cout
					<< "***info: output file is "
					<< out
					<< std::endl
					;
				process(body, out.parent_path() / "index.json", body_only);
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

		void driver::finish(boost::filesystem::path const& body, boost::filesystem::path const& out)
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
			//	mark end of symbol stream
			//
			//buffer_.emplace_back(symbol(SYM_EOF, "EOF"));

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

				if (verbose_ > 1)
				{
					//
					//	automatic generated meta data
					//
					std::cout
						<< "***info: last write time: "
						<< cyng::io::to_str(cyng::make_object(last_write_time_))
// 						;
// 					cyng::time_format(std::cout, last_write_time_);
						
// 					std::cout
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
					const double entropy = calculate_entropy(stats_);
					std::cout
						<< "***info: entropy is "
						<< entropy
						<< " (calculated over "
						<< calculate_size(stats_)
						<< " input token)"
						<< std::endl
						;
				}

				//	1. and 2. pass complete
				//	start compiler
				cyng::vector_t doc;
				doc.reserve(buffer_.size());	//	that's an estimation

				docscript::compiler c(buffer_, /*doc, */verbose_);
				c.run(last_write_time_, file_size_, out);

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
					//std::cout
						//<< cyng::to_literal(obj)
						//<< ((cyng::type_test< cyng::op >(obj)) ? "\n" : " ")
						;
				}
	#endif
				//
				//	serialize as program not as data (reverse on stack)
				//
				std::for_each(prg.begin(), prg.end(), [&file](cyng::object const& obj) {
// 					cyng::serialize_native(file, obj, cyng::io::custom_callback());
				});
			}

		}

		void driver::process(boost::filesystem::path const& in, boost::filesystem::path out, bool body_only)
		{
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
				gen.run(prg);
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

				//
				//	generate meta 
				//
				gen.meta(out.parent_path() / "meta.json");
			}
		}

		std::tuple<std::chrono::system_clock::time_point, uintmax_t> read_meta_data(boost::filesystem::path p)
		{
			return (boost::filesystem::exists(p) && boost::filesystem::is_regular(p))
				? std::make_tuple(std::chrono::system_clock::from_time_t(boost::filesystem::last_write_time(p)), boost::filesystem::file_size(p))
				: std::make_tuple(std::chrono::system_clock::now(), 0ul)
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
