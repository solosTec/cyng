/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/intrinsics/traits/tag_names.hpp>


namespace cyng 
{
	namespace traits
	{
		static const char * names[std::tuple_size<traits::tag_t>::value + 1] = {
			"null",
			
			"bool",
			"char",
			"f",	//	float
			"d",	//	double
			"ld",	//	long double
			"s",	//	const char*
			"u8", 	//	std::uint8_t
			"u16", 	// 	std::uint16_t
			"u32", 	//	uint32_t
			"u64", //	uint64_t
			"i8", 	// 	int8_t
			"i16", 	//	int16_t
			"i32", 	//	int32_t
			"i64",	//	int64_t
			"s",	//	string
			"chrono:tp",	//	chrono::system_clock::time_point
			"chrono:ns",	//	chrono::nanoseconds
			"chrono:us",	//	chrono::microseconds
			"chrono:ms",	//	chrono::milliseconds
			"chrono:sec",	//	chrono::seconds
			"chrono:min",	//	chrono::minutes
			"chrono:h",		//	chrono::hours
			
			"chrono:dtp",		//	chrono::dbl_time_point
			"v",		//	version
			"rev",		//	revision
			"op",		//	code
			"label",	//	jump address
			"log:severity",	//	logging::severity,
			"binary",	//	buffer_t
			"mac48",
			"mac64",	
			"#",		//	RGB color-8
			"#",		//	RGB color-16

			"crypto:MD5",
			"crypto:SHA1",
			"crypto:SHA256",
			"crypto:SHA512",
			
			"obj",		//	object
			
			"tpl",		//	tuple_t
			"vec",		//	vector_t
			"set",		//	set_t

			"amap",		//	attr_map_t - std::map<std::size_t, object>;
			"attr",		//	attr_t - std::pair<std::size_t, object>;
			"pmap",		//	param_map_t - std::map<std::string, object>;
			"param",	//	param_t - std::pair<std::string, object>;
			
			"lockable",	
			
			"ec",		//	boost::system::error_code
			"uuid",		//	boost::uuids::uuid
 			"fs:path",	//	boost::filesystem::path
			
			"ip:tcp:endpoint",	//	boost::asio::ip::tcp::endpoint
			"ip:udp:endpoint",	//	boost::asio::ip::udp::endpoint
			"ip:icmp:endpoint"	//	boost::asio::ip::icmp::endpoint
			"ip:address",		//	boost::asio::ip::address

			"eod",		//	nomen est omen
			"COSTUM"
		};
		
		const char* get_type_name(std::size_t idx)
		{
			return (idx < std::tuple_size<traits::tag_t>::value)
			? names[idx]
			: nullptr
// 			: "CUSTOM"
			;
		}
		
	}
}


