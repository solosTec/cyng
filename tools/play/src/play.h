/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_PLAY_H
#define CYNG_PLAY_H

#include <cstdint>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include <vector>

namespace cyng
{
	/**
	 * Read a file binary and send conetnt to a specified ip address.
	 */
	class play 
	{
	public:
		play(boost::filesystem::path const&, int verbose, std::size_t);
		int run(boost::asio::ip::tcp::endpoint ep, std::size_t);

	private:
		const boost::filesystem::path inp_;
		const int verbose_;
		std::vector<char> buffer_;
	};
}

#endif	//	CYNG_UNLOG_CU_H
