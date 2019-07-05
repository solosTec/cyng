/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_FILE_SYSTEM_HPP
#define CYNG_FILE_SYSTEM_HPP

//#include <CYNG_project_info.h>
#include <cstddef>
#include <utility>
#if defined(__CPP_SUPPORT_P0218R1)
#include <filesystem>
#else
#include <boost/filesystem.hpp>
#endif

namespace cyng 
{
	namespace file
	{
		
#if defined(__CPP_SUPPORT_P0218R1)

		/**
		 * File system library
		 */
		using path = std::filesystem::path;
		const auto absolute = std::filesystem::absolute;
		const auto begin = std::filesystem::begin;

#else
		using path = boost::filesystem::path;
		const auto absolute = boost::filesystem::absolute;
		const auto begin = boost::filesystem::begin;

#endif
		
	}	
}

#endif 
