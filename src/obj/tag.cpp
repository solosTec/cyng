/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#include <cyng/obj/tag.hpp>

namespace cyng {
}

namespace std {

	size_t hash<boost::system::error_code>::operator()(boost::system::error_code const& ec) const noexcept {
		return ec.value();
	}
	size_t hash<std::filesystem::path>::operator()(std::filesystem::path const& p) const noexcept {
		return boost::hash<std::filesystem::path>()(p);
	}
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
