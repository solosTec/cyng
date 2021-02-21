/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#include <cyng/parse/version.h>

#include <algorithm>

#include <boost/assert.hpp>
#include <boost/algorithm/string.hpp>

namespace cyng {

	version to_version(std::string const& str) {

		//
		//	expected format is "m.m"
		//
		std::vector<std::string> parts;
		boost::split(parts, str, boost::is_any_of("."));

		if (parts.size() == 2) {
			return version(
				static_cast<std::uint16_t>(std::stoul(parts.at(0), nullptr, 10)),
				static_cast<std::uint16_t>(std::stoul(parts.at(1), nullptr, 10))
			);
		}

		return version();
	}

	revision to_revision(std::string const& str) {

		//
		//	expected format is "m.m.p.t"
		//
		std::vector<std::string> parts;
		boost::split(parts, str, boost::is_any_of(":"));

		if (parts.size() == 4) {
			return revision(
				static_cast<std::uint16_t>(std::stoul(parts.at(0), nullptr, 10)),
				static_cast<std::uint16_t>(std::stoul(parts.at(1), nullptr, 10)),
				static_cast<std::uint16_t>(std::stoul(parts.at(2), nullptr, 10)),
				static_cast<std::uint16_t>(std::stoul(parts.at(3), nullptr, 10))
			);
		}
		return revision();

	}

}

