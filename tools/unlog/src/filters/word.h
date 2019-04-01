/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_UNLOG_WORD_H
#define CYNG_UNLOG_WORD_H

#include <cstdint>
#include <boost/filesystem.hpp>

namespace cyng
{
	/**
	 * Read input generated with hexdump command
	 *
	 * example
	 * 0007180 0228 a0d0 5772 e152 d6ca 3633 1df0 781c
	 * 0007190 1bc8 76b9 2c1f 78f5 b574 5d94 74aa 4c53
	 */
	class word 
	{
	public:
		word(boost::filesystem::path const&, boost::filesystem::path const&, int verbose);
		int run(std::size_t min, std::size_t max);

	private:

	private:
		const boost::filesystem::path inp_, out_;
		const int verbose_;
	};
}

#endif
