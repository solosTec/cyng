/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_UNLOG_CU_H
#define CYNG_UNLOG_CU_H

#include <cstdint>
#include <boost/filesystem.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/string_generator.hpp>

namespace cyng
{
	/**
	 * CU is proprietary log format from previous incarnations of this software.
	 * Example:
	 [20180703T025101.864255] INFO  580 -- 52e84cf9-733b-4ab5-906e-bc84f48d59c2@cu:1 received 242 bytes in state 3
	 [20180703T025101.864255] TRACE 580 -- 52e84cf9-733b-4ab5-906e-bc84f48d59c2@cu:1
	 [0000]  7e a8 f0 41 00 02 92 ef  14 35 7b 06 00 00 00 00  ~..A.... .5{.....
	 [0010]  02 06 09 0c 07 e2 07 02  01 08 2d 00 ff 80 00 08  ........ ..-.....
	 [0020]  11 08 06 01 3d a9 9a 06  00 a3 de 16 06 00 00 00  ....=... ........
	 [0030]  00 06 00 00 00 00 02 06  09 0c 07 e2 07 02 01 09  ........ ........
	 *
	 * First comes a timestamp with a UUID. After that a list with hex values.
	 * Each UUID can occure several times and the content will be written into
	 * separate files.
	 */
	class cu 
	{
	public:
		cu(boost::filesystem::path const&, boost::filesystem::path const&, int verbose);
		int run(std::size_t min, std::size_t max);

	private:
		void read_line(std::string const&, std::size_t);

	private:
		const boost::filesystem::path inp_, out_;
		const int verbose_;
		boost::uuids::uuid tag_;
		boost::uuids::string_generator sgen_;
	};
}

#endif	//	CYNG_UNLOG_CU_H
