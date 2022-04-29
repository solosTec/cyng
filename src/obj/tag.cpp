/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#include <cyng/obj/tag.hpp>

#include <algorithm>
#include <iterator>

#include <boost/algorithm/string.hpp>

namespace cyng {

	bool type_code_exists(std::string name) {
		return std::find_if(std::begin(traits::names), std::end(traits::names), [name](std::string const& cmp) {
			return boost::algorithm::equals(name, cmp);
		}) != std::end(traits::names);
	}

	type_code type_code_by_name(std::string name) {
		auto const pos = std::find_if(std::begin(traits::names), std::end(traits::names), [name](std::string const& cmp) {
			return boost::algorithm::equals(name, cmp);
		});

		if (pos != std::end(traits::names)) {
			auto const idx = std::distance(std::begin(traits::names), pos);
			return static_cast<type_code>(idx);
		}

		//	not found
		return TC_NULL;
	}

}

namespace std {

	size_t hash<boost::system::error_code>::operator()(boost::system::error_code const& ec) const noexcept {
		return ec.value();
	}
#if defined(_MSC_VER) && (_MSC_VER < 1931) || !defined(_MSC_VER)
	size_t hash<std::filesystem::path>::operator()(std::filesystem::path const& p) const noexcept {
		return boost::hash<std::filesystem::path>()(p);
	}
#endif
	size_t hash<boost::uuids::uuid>::operator()(boost::uuids::uuid const& tag) const noexcept {
		return boost::hash<boost::uuids::uuid>()(tag);
	}
	size_t hash<chrono::system_clock::time_point>::operator()(chrono::system_clock::time_point const& tp) const noexcept {
		using D = chrono::system_clock::time_point::duration;
		return hash<D>()(tp.time_since_epoch());
	}

#if !defined(BOOST_ASIO_HAS_STD_HASH)
	size_t hash<boost::asio::ip::address>::operator()(boost::asio::ip::address const& addr) const noexcept {
		return boost::hash<std::string>()(addr.to_string());
	}
#endif

}
