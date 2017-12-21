/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <CYNG_project_info.h>
#include <cyng/intrinsics/policy/hash.h>
#include <functional>
#include <boost/functional/hash.hpp>

namespace std 
{
	size_t hash<cyng::null>::operator()(cyng::null) const noexcept
	{
		return CYNG_NULL_HASH;
	}

	size_t hash<cyng::eod>::operator()(cyng::eod) const noexcept
	{
		return CYNG_EOD_HASH;
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
		return boost::hash_value(static_cast<std::uint16_t>(v));
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
	size_t hash<std::pair<long unsigned int, const char*>>::operator()(std::pair<long unsigned int, const char*> const& v) const noexcept
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
	
	size_t hash<boost::filesystem::path>::operator()(boost::filesystem::path const& v) const noexcept
	{
		return boost::hash<boost::filesystem::path>()(v);
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

}

namespace cyng
{
	
	std::size_t hash_value(object const& obj)
	{
		return obj.hash();
	}
}

