/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_FILE_SYSTEM_HPP
#define CYNG_FILE_SYSTEM_HPP

#include <cstddef>
#include <utility>
#if defined(__CPP_SUPPORT_P0218R1)
#include <filesystem>
#else
#include <boost/filesystem.hpp>
#endif

namespace cyng 
{
	namespace filesystem
	{
		
#if defined(__CPP_SUPPORT_P0218R1)

		/**
		 * File system library
		 */
		using path = std::filesystem::path;
		//const auto absolute = std::filesystem::absolute;
		//const auto begin = std::filesystem::begin;
		//const auto create_directory = std::filesystem::create_directory; //(const std::filesystem::path& p, std::error_code& ec);

		//
		//	this works fine
		//
		using F = bool (*) (const std::filesystem::path&);
		const F create_directory = std::filesystem::create_directory;

		//
		//	this not so
		//
		//const auto create_directory = std::filesystem::create_directory;

#else
		using path = boost::filesystem::path;
		//const auto absolute = boost::filesystem::absolute;
		//const auto begin = boost::filesystem::begin;
		//const auto create_directory = boost::filesystem::create_directory;

#endif
		
	}	
}

#endif 
