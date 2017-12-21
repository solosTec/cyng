/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_DOCC_READER_H
#define CYNG_DOCC_READER_H

#include <cstdint>
#include <boost/filesystem.hpp>

namespace cyng
{
	namespace docscript
	{
		class driver;
		class reader 
		{
		public:
			reader(driver&, boost::filesystem::path const&);
			bool run(std::size_t depth);

		private:
			boost::filesystem::path parse_include(std::string const& line);
			void tokenize(std::string const& );

		private:
			driver& driver_;
			boost::filesystem::path const& source_;
			std::size_t	line_;
		};
	}
}

#endif	//	CYNG_DOCC_READER_H
