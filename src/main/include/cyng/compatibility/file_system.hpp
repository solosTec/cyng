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
#include <cyng/rnd.h>

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


#else
		/**
		 * File system library
		 */
		using namespace boost::filesystem;

		using path = boost::filesystem::path;
		using space_info = boost::filesystem::space_info;

#endif
		
	}	
}

#endif 
