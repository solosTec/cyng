/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 


#include "batch.h"
#include "../../src/driver.h"
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>

namespace cyng
{
	namespace docscript
	{
	
		batch::batch(std::vector< std::string >const& inc
			, int verbose)
		: includes_(inc.begin(), inc.end())
			, verbose_(verbose)
		{}
		
		batch::~batch()
		{}

		int batch::run(boost::filesystem::path const& inp
			, boost::filesystem::path const& out)
		{
			if (boost::filesystem::is_directory(inp)) {

				for (auto const& entry : boost::make_iterator_range(boost::filesystem::directory_iterator(inp), {})) {

					process_file(entry, out);
				}

				//
				//	generate index file
				//
				generate_index(out);

				return EXIT_SUCCESS;
			}			

			//
			//	not a directory
			//
			std::cout << "***error: " << inp << " is not a directory" << std::endl;
			return EXIT_FAILURE;
		}

		void batch::process_file(boost::filesystem::path const& inp
			, boost::filesystem::path const& out)
		{
			//
			//	exclude all files without the extension ".docscript"
			//
			if (inp.has_extension()) {

				if (boost::algorithm::equals(".docscript", inp.extension().string())) {

					//
					//	generate some temporary file names for intermediate files
					//
					boost::filesystem::path tmp = boost::filesystem::temp_directory_path() / (boost::filesystem::path(inp).filename().string() + ".bin");

					//
					//	Construct driver instance
					//
					driver d(includes_, verbose_);

					//
					//	output file
					//
					boost::filesystem::path out_file = out / inp.filename();
					out_file.replace_extension(".html");

					if (verbose_ > 0) {
						std::cout << "***info: processing file " 
							<< inp 
							<< " ==> "
							<< out_file
							<< std::endl;
					}

					//
					//	Start driver with the main/input file
					//
					d.run(boost::filesystem::path(inp).filename()
						, tmp
						, cyng::docscript::verify_extension(out_file, "html")
						, true	//	only HTML body
						, true);	//	generate meta data

				}
			}
		}

		void batch::generate_index(boost::filesystem::path const& out)
		{
			if (boost::filesystem::is_directory(out)) {

				for (auto const& entry : boost::make_iterator_range(boost::filesystem::directory_iterator(out), {})) {

					read_meta_data(entry);
					//process_file(entry, out);
				}
			}
		}

		void batch::read_meta_data(boost::filesystem::path const& p)
		{

			//
			//	exclude all files without the extension ".json"
			//
			if (p.has_extension()) {

				if (boost::algorithm::equals(".json", p.extension().string())) {

					if (verbose_ > 1) {
						std::cout << "***info: read meta data "
							<< p
							<< std::endl;
					}
				}
			}
		}
	}	
}
