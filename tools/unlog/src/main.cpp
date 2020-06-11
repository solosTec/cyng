/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#include <boost/program_options.hpp>
#include <cyng/compatibility/file_system.hpp>
#include <boost/config.hpp>
#include <boost/predef.h>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <limits>
#include <CYNG_project_info.h>
#include "filters/hexdump.h"
#include "filters/hexdump_cpp.h"
#include "filters/cu.h"
#include "filters/word.h"
#include "filters/restore.h"
#if BOOST_OS_WINDOWS
#define NOMINMAX
#include <windows.h>
#endif

/**
 * Decrypt log files.
 * Read a log file of several formats and convert into different formats include 
 * the original binary file.
 *
 * Start with
 * @code
 * build/unlog -hexdump app0.log
 * @endcode
 */
int main(int argc, char* argv[]) {

	try
	{
		auto const cwd = cyng::filesystem::current_path();
		auto const temp = cyng::filesystem::temp_directory_path();

		std::string config_file;
#ifdef _DEBUG
		std::string inp_file = std::string(CYNG_SOURCE_DIRECTORY) + "/tools/unlog/examples/app0.log";
#else 
		std::string inp_file = "input.log";
#endif
		std::string out_file = (cwd / "out.bin").string();
		std::string filter = "hexdump";

		//
		//	generic options
		//
		boost::program_options::options_description generic("Generic options");
		generic.add_options()

			("help,h", "print usage message")
			("version,v", "print version string")
			("build,b", "last built timestamp and platform")
			("config,C", boost::program_options::value<std::string>(&config_file)->default_value("unlog.cfg"), "configuration file")

			;

		//
		//	all unlog options
		//
		boost::program_options::options_description unlog("unlog");
		unlog.add_options()

			("source,S", boost::program_options::value(&inp_file)->default_value(inp_file), "source file")
			("output,O", boost::program_options::value(&out_file)->default_value(out_file), "output file")
			//	verbose level
			("verbose,V", boost::program_options::value<int>()->default_value(0)->implicit_value(1), "verbose level")
			("filter,F", boost::program_options::value(&filter)->default_value(filter), "the filter to apply")
			("begin,B", boost::program_options::value<std::size_t>()->default_value(std::numeric_limits<std::size_t>::min())->implicit_value(std::numeric_limits<std::size_t>::min()), "line to start")
			("end,E", boost::program_options::value<std::size_t>()->default_value(std::numeric_limits<std::size_t>::max())->implicit_value(std::numeric_limits<std::size_t>::max()), "max. line number")
			;

		//
		//	all you can grab from the command line
		//
		boost::program_options::options_description cmdline_options;
		cmdline_options.add(generic).add(unlog);

		//
		//	positional arguments
		//
		boost::program_options::positional_options_description p;
		p.add("source", -1);

		boost::program_options::variables_map vm;
		boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(cmdline_options).positional(p).run(), vm);
		boost::program_options::notify(vm);

		if (vm.count("help"))
		{
			std::cout
				<< cmdline_options
				<< std::endl
				;
			return EXIT_SUCCESS;
		}

		if (vm.count("version"))
		{
			std::cout
				<< "unlog v"
				<< CYNG_VERSION
				<< std::endl
				;
			return EXIT_SUCCESS;
		}

		if (vm.count("build"))
		{
			std::cout
				<< "last built at : "
				<< __DATE__
				<< " "
				<< __TIME__
				<< std::endl
				<< "Platform      : "
				<< CYNG_PLATFORM
				<< std::endl
				<< "Compiler      : "
				<< BOOST_COMPILER
				<< std::endl
				<< "StdLib        : "
				<< BOOST_STDLIB
				<< std::endl

#if defined(BOOST_LIBSTDCXX_VERSION)
				<< "StdLib++      : "
				<< BOOST_LIBSTDCXX_VERSION
				<< std::endl
#endif

				<< "BOOSTLib      : "
				<< BOOST_LIB_VERSION
				<< " ("
				<< BOOST_VERSION
				<< ")"
				<< std::endl

// 				<< "build type    : "
// 				<< CYX_BUILD_TYPE
// 				<< std::endl
// 				<< "ODBC available: "
// #if defined(CYNG_ODBC_HEADER_AVAILABLE)
// 				<< "yes"
// #else
// 				<< "no"
// #endif
// 				<< std::endl
				<< "shared mutex  : "
#if defined(BOOST_NO_CXX14_HDR_SHARED_MUTEX)
				<< "no"
#else
				<< "yes"
#endif
				<< std::endl
				<< std::endl
				;
			return EXIT_SUCCESS;
		}


		std::ifstream ifs(config_file);
		if (!ifs)
		{
			std::cout
				<< "***info: config file: "
				<< config_file
				<< " not found"
				<< std::endl
				;
		}
		else
		{
			boost::program_options::store(boost::program_options::parse_config_file(ifs, unlog), vm);
			boost::program_options::notify(vm);
		}

		const int verbose = vm["verbose"].as< int >();
		if (verbose > 0)
		{
			std::cout
				<< "***info: verbose level: "
				<< verbose
				<< std::endl
				;
		}


#if BOOST_OS_WINDOWS
		//
		//	set console outpt code page to UTF-8
		//	requires a TrueType font like Lucida 
		//
		if (::SetConsoleOutputCP(65001) == 0)
		{
			std::cerr
				<< "Cannot set console code page"
				<< std::endl
				;

		}
#endif

		auto const begin = vm["begin"].as< std::size_t >();
		auto const end = vm["end"].as< std::size_t >();
		std::cout
			<< "***info: selected filter: "
			<< filter
			<< std::endl
			;

		//
		//	Construct selected filter
		//
		if (boost::algorithm::equals("hexdump", filter)) {
			//
			//	Example:
			//	[0000]  1b 1b 1b 1b 1b 1b 1b 1b  01 01 01 01 76 81 06 31  ........ ....v..1
			//	[0010]  39 30 38 32 33 32 30 33  38 33 38 38 39 32 36 32  90823203 83889262
			//	[0020]  35 2d 31 62 00 62 00 72  63 01 00 77 01 07 00 50  5-1b.b.r c..w...P

			return cyng::hexdump(inp_file, out_file, verbose).run(begin, end);
		}
		else if (boost::algorithm::equals("cpp", filter)) {
			if (vm["output"].defaulted()) {
				//
				//	Derive an output name from the input name
				//	if no name was explicitely defined.
				//
				cyng::filesystem::path tmp = inp_file;
				out_file = tmp.replace_extension(".ipp").string();

				if (verbose > 1)
				{
					std::cout
						<< "***info: output file is: "
						<< out_file
						<< std::endl
						;
				}
			}
			return cyng::hexdump_cpp(inp_file, out_file, verbose).run(begin, end);
		}
		else if (boost::algorithm::equals("cu", filter)) {
			if (vm["output"].defaulted()) {
				//
				//	Derive an output name from the input name
				//	if no name was explicitely defined.
				//temp
				cyng::filesystem::path tmp = inp_file;
				out_file = tmp.parent_path().string();

				if (verbose > 1)
				{
					std::cout
						<< "***info: output path is: "
						<< out_file
						<< std::endl
						;
				}
			}
			return cyng::cu(inp_file, out_file, verbose).run(begin, end);
		}
		else if (boost::algorithm::equals("word", filter)) {
			//
			//	output generated by 
			//	cat /dev/ttyAPP0 | hexdump 
			//
			//	example:
			//	0000000 4436 2324 0796 0020 0e63 2972 6543 e687
			//	0000010 bf1e b903 2000 4005 3ac8 a880 66e0 aa8c
			return cyng::word(inp_file, out_file, verbose).run(begin, end);
		}
		else if (boost::algorithm::equals("restore", filter)) {
			//
			//	read a binary file and create a hexdump
			//
			if (vm["output"].defaulted()) {
				//
				//	Derive an output name from the input name
				//	if no name was explicitely defined.
				//
				cyng::filesystem::path tmp = inp_file;
				out_file = tmp.replace_extension(".log").string();

				if (verbose > 1)
				{
					std::cout
						<< "***info: output file is: "
						<< out_file
						<< std::endl
						;
				}
			}
			return cyng::restore(inp_file, out_file, verbose).run(begin, end);
		}

		std::cerr << "unknown filter: " << filter << std::endl;

	}
	catch (std::exception& e)
	{
		std::cerr
			<< e.what()
			<< std::endl
			;
	}

	return EXIT_FAILURE;
		
}
