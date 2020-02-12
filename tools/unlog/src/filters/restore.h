/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_UNLOG_RESTORE_H
#define CYNG_UNLOG_RESTORE_H

#include <cstdint>
#include <boost/filesystem.hpp>

namespace cyng
{
	/**
	 * read a binary file and create a hexdump
	 */
	class restore 
	{
	public:
		restore(boost::filesystem::path const&, boost::filesystem::path const&, int verbose);
		int run(std::size_t min, std::size_t max);

	private:
		const boost::filesystem::path inp_, out_;
		const int verbose_;
	};
}

#endif
