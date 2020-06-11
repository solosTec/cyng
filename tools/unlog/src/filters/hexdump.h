/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_UNLOG_HEXDUMP_H
#define CYNG_UNLOG_HEXDUMP_H

#include <cstdint>
#include <cyng/compatibility/file_system.hpp>

namespace cyng
{
	/**
	 * Read input generated with hexdump command: hexdump -x
	 */
	class hexdump 
	{
	public:
		hexdump(filesystem::path const&, filesystem::path const&, int verbose);
		int run(std::size_t min, std::size_t max);

	private:

	private:
		const filesystem::path inp_, out_;
		const int verbose_;
	};
}

#endif	//	CYNG_UNLOG_HEXDUMP_H
