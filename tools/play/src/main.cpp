/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/config.hpp>
#include <boost/predef.h>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <limits>
#include <CYNG_project_info.h>
#if BOOST_OS_WINDOWS
#define NOMINMAX
#endif
#include <boost/asio.hpp>
#include "play.h"

/**
 * Send the content of a file to a specified IP address.
 *
 * Start with
 * @code
 * build/play app.bin
 * @endcode
 */
int main(int argc, char* argv[]) {

	try
	{
		const boost::filesystem::path cwd = boost::filesystem::current_path();
		const boost::filesystem::path temp = boost::filesystem::temp_directory_path();

		std::string config_file;
#ifdef _DEBUG
		std::string inp_file = std::string(CYNG_SOURCE_DIRECTORY) + "/tools/unlog/examples/app0.log";
#else 
		std::string inp_file = "input.log";
#endif
		std::string address = "127.0.0.1";

		//
		//	generic options
		//
		boost::program_options::options_description generic("Generic options");
		generic.add_options()

			("help,h", "print usage message")
			("version,v", "print version string")
			("build,b", "last built timestamp and platform")
			("config,C", boost::program_options::value<std::string>(&config_file)->default_value("play.cfg"), "configuration file")

			;

		//
		//	all unlog options
		//
		boost::program_options::options_description play("play");
		play.add_options()

			("source,S", boost::program_options::value(&inp_file)->default_value(inp_file), "source file")
			("address,A", boost::program_options::value(&address)->default_value(address), "tcp/ip address")
			("port,P", boost::program_options::value<std::uint16_t>()->default_value(80)->implicit_value(8080), "ip port")
			//	verbose level
			("verbose,V", boost::program_options::value<int>()->default_value(0)->implicit_value(1), "verbose level")
			//("begin,B", boost::program_options::value<std::size_t>()->default_value(std::numeric_limits<std::size_t>::min())->implicit_value(std::numeric_limits<std::size_t>::min()), "line to start")
			//("end,E", boost::program_options::value<std::size_t>()->default_value(std::numeric_limits<std::size_t>::max())->implicit_value(std::numeric_limits<std::size_t>::max()), "max. line number")
			;

		//
		//	all you can grab from the command line
		//
		boost::program_options::options_description cmdline_options;
		cmdline_options.add(generic).add(play);

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
			boost::program_options::store(boost::program_options::parse_config_file(ifs, play), vm);
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

		//
		//	Construct endpoint
		//
		const std::uint16_t port = vm["port"].as< std::uint16_t >();
		boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(address), port);
		if (verbose > 1)
		{
			std::cout
				<< "***info: enpoint: "
				<< ep
				<< std::endl
				;
		}
		return cyng::play(inp_file, verbose).run(ep);


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
