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
		using space_info = std::filesystem::space_info;

		//
		//	this works fine
		//
		using FN_CREATE_DIR = bool (*) (std::filesystem::path const&);
		const FN_CREATE_DIR create_directory = std::filesystem::create_directory;

		using FN_SPACE = space_info (*) (std::filesystem::path const&);
		const FN_SPACE space = std::filesystem::space;

		//
		//	this not so
		//
		//const auto create_directory = std::filesystem::create_directory;

#else
		using path = boost::filesystem::path;
		using space_info = boost::filesystem::space_info;
		//const auto absolute = boost::filesystem::absolute;
		//const auto begin = boost::filesystem::begin;
		//const auto create_directory = boost::filesystem::create_directory;


#endif
		
	}	
}

#endif 
