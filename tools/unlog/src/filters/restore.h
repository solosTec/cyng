/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_UNLOG_RESTORE_H
#define CYNG_UNLOG_RESTORE_H

#include <cstdint>
#include <cyng/compatibility/file_system.hpp>

namespace cyng
{
	/**
	 * read a binary file and create a hexdump
	 */
	class restore 
	{
	public:
		restore(filesystem::path const&, filesystem::path const&, int verbose);
		int run(std::size_t min, std::size_t max);

	private:
		const filesystem::path inp_, out_;
		const int verbose_;
	};
}

#endif
