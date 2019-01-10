/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_DOCC_BATCH_H
#define CYNG_DOCC_BATCH_H

#include <cyng/docscript/tokenizer.h>
#include <cyng/docscript/lexer.h>
#include <cyng/docscript/statistics.h>
#include <cyng/intrinsics/sets.h>
#include <chrono>
#include <boost/filesystem.hpp>

namespace cyng
{
	namespace docscript
	{
		class reader;

		/**
		 * Compiles all docScript files in the specified directory und uses the generated
		 * meta data to compile a an index to navigate to all generated output files.
		 */
		class batch
		{

		public:
			/**
			 * Constructor
			 *
			 * @param inc vector of include paths
			 * @param verbose verbose level. The higher the number, the more will be logged.
			 */
			batch(std::vector< std::string >const& inc, int verbose);
			virtual ~batch();

			/**
			 * In a first step the compiler generates an intermediate file from all
			 * input files that contains instructions to generate the output file.
			 * In a second step a special VM executes the instructions to generate the
			 * requested output file (HTML, PDF, ...)
			 *
			 * @param master master file
			 * @param tmp temporary intermediate file. 
			 * @param out output file (html)
			 */
			int run(boost::filesystem::path const& inp
				, boost::filesystem::path const& out
				, bool gen_robot
				, bool gen_sitemap);

		private:
			void process_file(boost::filesystem::path const& inp
				, boost::filesystem::path const& out);

			void generate_index(boost::filesystem::path const& out
				, bool gen_robot
				, bool gen_sitemap);
			void generate_index_page(boost::filesystem::path const& out
				, std::map<std::chrono::system_clock::time_point, tuple_t> const&);
			void generate_index_map(boost::filesystem::path const& out
				, std::map<std::chrono::system_clock::time_point, tuple_t> const&);
			void generate_robots_txt(boost::filesystem::path const& out
				, std::map<std::chrono::system_clock::time_point, tuple_t> const&
				, bool gen_sitemap);
			void generate_sitemap(boost::filesystem::path const& out
				, std::map<std::chrono::system_clock::time_point, tuple_t> const&);
			object read_meta_data(boost::filesystem::path const& p);

		private:
			/**
			 * Manage a list of include directories.
			 * If the driver opens a file it searches in all given directories
			 * until the spcified file is found.
			 */
			std::vector< std::string > const includes_;

			/**
			 * verbosity level.
			 * 0 == print only errors
			 */
			int const verbose_;

		};

	}
}

#endif	//	CYNG_DOCC_DRIVER_H
