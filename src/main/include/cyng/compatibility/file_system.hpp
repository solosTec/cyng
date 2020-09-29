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
#include <cyng/compatibility/general.h>
#if defined(__CPP_SUPPORT_P0218R1)
#include <filesystem>
#else
#include <boost/filesystem.hpp>
#include <chrono>
#endif
#include <cyng/rnd.h>
#include <cyng/util/split.h>
#include <boost/predef.h>

namespace cyng 
{

	namespace filesystem
	{
		
#if defined(__CPP_SUPPORT_P0218R1)

		/**
		 * File system library
		 */
		using namespace std::filesystem;
		using path = std::filesystem::path;
		using space_info = std::filesystem::space_info;

		/**
		 * substitutes similiar function from boost
		 */
		inline typename path::string_type unique_path(const path& model)
		{
			typename path::string_type s(model.native());

			char const percent = '%';
			auto rnd = crypto::make_rnd_alnum();

			for (path::string_type::size_type i = 0; i < s.size(); ++i)
			{
				if (s[i] == percent) {
					s[i] = rnd.next();	// substitute
				}
			}

			return s;
		}

		/**
		 * renaming function from boost
		 */
		inline bool is_regular(std::filesystem::path const& p)
		{
			return is_regular_file(p);
		}

		/**
		 * To get the last write time is currently a mess with the STL.
		 * Waiting for C++20
		 */
		inline std::chrono::system_clock::time_point get_write_time(path const& p)
		{
			auto const ftp = cyng::filesystem::last_write_time(p);
			auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(ftp - std::filesystem::file_time_type::clock::now()
				+ std::chrono::system_clock::now());
			auto const tt = std::chrono::system_clock::to_time_t(sctp);
			return std::chrono::system_clock::from_time_t(tt);
		}

		/**
		 * Generic filenames not really works - so here is a substitute to convert UNIX path to windows path
		 */
		inline std::filesystem::path make_path(std::string const& str) {
#if BOOST_OS_WINDOWS
			std::filesystem::path p;
			auto const vec = split(str, "/");
			for (auto const s : vec) {
				p /= s;
			}
			return p;
#else
			return std::filesystem::path(s);
#endif

		}

#else
		/**
		 * File system library
		 */
		using namespace boost::filesystem;

		using path = boost::filesystem::path;
		using space_info = boost::filesystem::space_info;

		//
		//	same function for boost and STL
		//
		inline std::chrono::system_clock::time_point get_write_time(path const& p)
		{
			const auto tt = filesystem::last_write_time(p);
			return std::chrono::system_clock::from_time_t(tt);
		}

		inline std::filesystem::path make_path(std::string const& str) {
			return std::filesystem::path(s);
		}

#endif
		
	}	
}

#endif 
