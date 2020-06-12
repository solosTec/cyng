/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <CYNG_project_info.h>
#include <cyng/intrinsics/policy/hash.h>
#include <boost/functional/hash.hpp>

namespace std 
{
	size_t hash<cyng::null>::operator()(cyng::null) const noexcept
	{
		return CYNG::null_hash;
	}

	size_t hash<cyng::eod>::operator()(cyng::eod) const noexcept
	{
		return CYNG::eod_hash;
	}

	size_t hash<cyng::version>::operator()(cyng::version const& v) const noexcept
	{
        return hash<cyng::version::full_type>()(v.full());
	}

	size_t hash<cyng::revision>::operator()(cyng::revision const& v) const noexcept
	{
        return hash<cyng::revision::full_type>()(v.full());
	}
	
	size_t hash<cyng::color_8>::operator()(cyng::color_8 const& v) const noexcept
	{
		std::size_t seed = 0;
		boost::hash_combine(seed, v.red());
		boost::hash_combine(seed, v.green());
		boost::hash_combine(seed, v.blue());
		boost::hash_combine(seed, v.opacity());
		return seed;
	}

	size_t hash<cyng::color_16>::operator()(cyng::color_16 const& v) const noexcept
	{
		std::size_t seed = 0;
		boost::hash_combine(seed, v.red());
		boost::hash_combine(seed, v.green());
		boost::hash_combine(seed, v.blue());
		boost::hash_combine(seed, v.opacity());
		return seed;
	}
	
	size_t hash<cyng::mac48>::operator()(cyng::mac48 const& v) const noexcept
	{
		return boost::hash_value(v.get_octets());
	}
	
	size_t hash<cyng::mac64>::operator()(cyng::mac64 const& v) const noexcept
	{
		return boost::hash_value(v.get_words());
	}
	
	size_t hash<cyng::code>::operator()(cyng::code v) const noexcept
	{
        return boost::hash_value(static_cast<std::uint8_t>(v));
	}
	
    size_t hash<cyng::logging::severity>::operator()(cyng::logging::severity v) const noexcept
    {
        return boost::hash_value(static_cast<std::uint16_t>(v));
    }

    size_t hash<cyng::label>::operator()(cyng::label const& v) const noexcept
	{
		return boost::hash_value(v.name_);
	}

	size_t hash<cyng::buffer_t>::operator()(cyng::buffer_t const& v) const noexcept
	{
		return boost::hash_value(v);
	}
	
	size_t hash<cyng::tuple_t>::operator()(cyng::tuple_t const& v) const noexcept
	{
		return boost::hash_value(v);
	}
	
	size_t hash<cyng::vector_t>::operator()(cyng::vector_t const& v) const noexcept
	{
		return boost::hash_value(v);
	}

	size_t hash<cyng::set_t>::operator()(cyng::set_t const& v) const noexcept
	{
		return boost::hash_value(v);
	}

	size_t hash<cyng::attr_map_t>::operator()(cyng::attr_map_t const& v) const noexcept
	{
		return boost::hash_value(v);
	}

	size_t hash<cyng::attr_t>::operator()(cyng::attr_t const& v) const noexcept
	{
		std::size_t seed = 0;
		boost::hash_combine(seed, v.first);
		boost::hash_combine(seed, v.second.hash());
		return seed;
	}

	size_t hash<cyng::param_map_t>::operator()(cyng::param_map_t const& v) const noexcept
	{
        return boost::hash_value(v);
	}

	size_t hash<cyng::param_t>::operator()(cyng::param_t const& v) const noexcept
	{
		std::size_t seed = 0;
		boost::hash_combine(seed, v.first);
		boost::hash_combine(seed, v.second.hash());
		return seed;
	}
	
	size_t hash<std::pair<std::size_t, std::string>	>::operator()(std::pair<std::size_t, std::string> const& v) const noexcept
	{
		std::size_t seed = 0;
		boost::hash_combine(seed, v.first);
		boost::hash_combine(seed, v.second);
		return seed;
	}
	
#if defined(CYNG_LEGACY_MODE_ON)
	size_t hash<std::pair<long unsigned int, const char*> >::operator()(std::pair<long unsigned int, const char*> const& v) const noexcept
	{
		std::size_t seed = 0;
		boost::hash_combine(seed, v.first);
		boost::hash_combine(seed, v.second);
		return seed;
	}
#endif

	size_t hash<boost::uuids::uuid>::operator()(boost::uuids::uuid const& v) const noexcept
	{
		return boost::hash<boost::uuids::uuid>()(v);
	}
	
	size_t hash<cyng::filesystem::path>::operator()(cyng::filesystem::path const& v) const noexcept
	{
		return boost::hash<cyng::filesystem::path>()(v);
	}

	size_t hash<boost::system::error_code>::operator()(boost::system::error_code const& v) const noexcept
	{
		return boost::hash<boost::system::error_code>()(v);
	}

	size_t hash<chrono::system_clock::time_point>::operator()(chrono::system_clock::time_point const& v) const noexcept
	{
		using D = chrono::system_clock::time_point::duration;
// 		using P = chrono::system_clock::time_point::period;
// 		using R = chrono::system_clock::time_point::rep;
		return hash<D>()(v.time_since_epoch());
	}
	
	size_t hash<cyng::chrono::dbl_time_point>::operator()(cyng::chrono::dbl_time_point const& v) const noexcept
	{
		std::size_t seed = 0;
		boost::hash_combine(seed, v.first);
		boost::hash_combine(seed, v.second);
		return seed;
	}

	size_t hash<cyng::crypto::digest_md5>::operator()(cyng::crypto::digest_md5 const& v) const noexcept
	{
		return boost::hash<cyng::crypto::digest_md5::value_type>()(v.data_);
	}

	size_t hash<cyng::crypto::digest_sha1>::operator()(cyng::crypto::digest_sha1 const& v) const noexcept
	{
		return boost::hash<cyng::crypto::digest_sha1::value_type>()(v.data_);
	}

	size_t hash<cyng::crypto::digest_sha256>::operator()(cyng::crypto::digest_sha256 const& v) const noexcept
	{
		return boost::hash<cyng::crypto::digest_sha256::value_type>()(v.data_);
	}

	size_t hash<cyng::crypto::digest_sha512>::operator()(cyng::crypto::digest_sha512 const& v) const noexcept
	{
		return boost::hash<cyng::crypto::digest_sha512::value_type>()(v.data_);
	}

	size_t hash<cyng::crypto::aes_128_key>::operator()(cyng::crypto::aes_128_key const& v) const noexcept
	{
		return boost::hash<cyng::crypto::aes_128_key::key_type>()(v.key_);
	}

	size_t hash<cyng::crypto::aes_192_key>::operator()(cyng::crypto::aes_192_key const& v) const noexcept
	{
		return boost::hash<cyng::crypto::aes_192_key::key_type>()(v.key_);
	}

	size_t hash<cyng::crypto::aes_256_key>::operator()(cyng::crypto::aes_256_key const& v) const noexcept
	{
		return boost::hash<cyng::crypto::aes_256_key::key_type>()(v.key_);
	}

	
	size_t hash<boost::asio::ip::tcp::endpoint>::operator()(boost::asio::ip::tcp::endpoint const& v) const noexcept
	{
		std::size_t seed = 0;
		boost::hash_combine(seed, v.address().to_string());
		boost::hash_combine(seed, v.port());
		return seed;
	}

	size_t hash<boost::asio::ip::udp::endpoint>::operator()(boost::asio::ip::udp::endpoint const& v) const noexcept
	{
		std::size_t seed = 0;
		boost::hash_combine(seed, v.address().to_string());
		boost::hash_combine(seed, v.port());
		return seed;
	}

	size_t hash<boost::asio::ip::icmp::endpoint>::operator()(boost::asio::ip::icmp::endpoint const& v) const noexcept
	{
		std::size_t seed = 0;
		boost::hash_combine(seed, v.address().to_string());
		boost::hash_combine(seed, v.port());
		return seed;
	}

	size_t hash<boost::asio::ip::address>::operator()(boost::asio::ip::address const& v) const noexcept
	{
		return boost::hash<std::string>()(v.to_string());
	}

#if defined(CYNG_ODBC_INSTALLED)
	size_t hash<SQL_TIMESTAMP_STRUCT>::operator()(SQL_TIMESTAMP_STRUCT const& ts) const noexcept
	{
		std::size_t seed = 0;
		boost::hash_combine(seed, ts.year);
		boost::hash_combine(seed, ts.month);
		boost::hash_combine(seed, ts.day);
		boost::hash_combine(seed, ts.hour);
		boost::hash_combine(seed, ts.minute);
		boost::hash_combine(seed, ts.second);
		boost::hash_combine(seed, ts.fraction);
		return seed;
	}
#endif

}

namespace cyng
{
	
	std::size_t hash_value(object const& obj)
	{
		return obj.hash();
	}
}

