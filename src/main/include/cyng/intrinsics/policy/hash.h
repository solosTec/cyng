/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_HASH_H
#define CYNG_HASH_H


#include <cyng/intrinsics.h>
#include <functional>

namespace std 
{
	template<> 
	struct hash<cyng::null>
	{
		size_t operator()(cyng::null) const noexcept;
	};
	
	template<> 
	struct hash<cyng::eod>
	{
		size_t operator()(cyng::eod) const noexcept;
	};

	template<> 
	struct hash<cyng::version>
	{
		size_t operator()(cyng::version const&) const noexcept;
	};

	template<> 
	struct hash<cyng::revision>
	{
		size_t operator()(cyng::revision const&) const noexcept;
	};

	template<> 
	struct hash<cyng::color_8>
	{
		size_t operator()(cyng::color_8 const&) const noexcept;
	};

	template<> 
	struct hash<cyng::color_16>
	{
		size_t operator()(cyng::color_16 const&) const noexcept;
	};

	template<> 
	struct hash<cyng::mac48>
	{
		size_t operator()(cyng::mac48 const&) const noexcept;
	};

	template<> 
	struct hash<cyng::mac64>
	{
		size_t operator()(cyng::mac64 const&) const noexcept;
	};

	template<> 
	struct hash<cyng::code>
	{
		size_t operator()(cyng::code) const noexcept;
	};

    template<>
    struct hash<cyng::logging::severity>
    {
        size_t operator()(cyng::logging::severity) const noexcept;
    };

    template<>
	struct hash<cyng::label>
	{
		size_t operator()(cyng::label const&) const noexcept;
	};

	template<>
	struct hash<cyng::buffer_t>
	{
		size_t operator()(cyng::buffer_t const&) const noexcept;
	};

	template<> 
	struct hash<cyng::tuple_t>
	{
		size_t operator()(cyng::tuple_t const&) const noexcept;
	};

	template<> 
	struct hash<cyng::vector_t>
	{
		size_t operator()(cyng::vector_t const&) const noexcept;
	};

	template<> 
	struct hash<cyng::set_t>
	{
		size_t operator()(cyng::set_t const&) const noexcept;
	};

	template<> 
	struct hash<cyng::attr_map_t>
	{
		size_t operator()(cyng::attr_map_t const&) const noexcept;
	};

	template<> 
	struct hash<cyng::attr_t>
	{
		size_t operator()(cyng::attr_t const&) const noexcept;
	};

	template<> 
	struct hash<cyng::param_map_t>
	{
		size_t operator()(cyng::param_map_t const&) const noexcept;
	};

	template<> 
	struct hash<cyng::param_t>
	{
		size_t operator()(cyng::param_t const&) const noexcept;
	};

// 	std::pair<std::size_t, std::string>	
	template<> 
	struct hash<std::pair<std::size_t, std::string>	>
	{
		size_t operator()(std::pair<std::size_t, std::string>	 const&) const noexcept;
	};
	
#if defined(CYNG_LEGACY_MODE_ON)
	template<> 
	struct hash<std::pair<long unsigned int, const char*>	>
	{
		size_t operator()(std::pair<long unsigned int, const char*>	 const&) const noexcept;
	};
#endif
	
	template<> 
	struct hash<boost::uuids::uuid>
	{
		size_t operator()(boost::uuids::uuid const&) const noexcept;
	};

	template<> 
	struct hash<boost::filesystem::path>
	{
		size_t operator()(boost::filesystem::path const&) const noexcept;
	};

	template<>
	struct hash<boost::system::error_code>
	{
		size_t operator()(boost::system::error_code const&) const noexcept;
	};

	template<>
	struct hash<chrono::system_clock::time_point>
	{
		size_t operator()(chrono::system_clock::time_point const&) const noexcept;
	};

	template<typename P, typename R>
    struct hash<chrono::duration<P, R>>
    {
        typedef chrono::duration<P, R> argument_type;
        typedef size_t result_type;
        result_type operator()(argument_type const& s) const
        {
            return hash<P>{}(s.count());
        }
    };	

	template<>
	struct hash<cyng::chrono::dbl_time_point>
	{
		size_t operator()(cyng::chrono::dbl_time_point const&) const noexcept;
	};

	template<>
	struct hash<cyng::crypto::digest_md5>
	{
		size_t operator()(cyng::crypto::digest_md5 const&) const noexcept;
	};
	
	template<>
	struct hash<cyng::crypto::digest_sha1>
	{
		size_t operator()(cyng::crypto::digest_sha1 const&) const noexcept;
	};
	
	template<>
	struct hash<cyng::crypto::digest_sha256>
	{
		size_t operator()(cyng::crypto::digest_sha256 const&) const noexcept;
	};

	template<>
	struct hash<cyng::crypto::digest_sha512>
	{
		size_t operator()(cyng::crypto::digest_sha512 const&) const noexcept;
	};

	template<>
	struct hash<boost::asio::ip::tcp::endpoint>
	{
		size_t operator()(boost::asio::ip::tcp::endpoint const&) const noexcept;
	};

	template<>
	struct hash<boost::asio::ip::udp::endpoint>
	{
		size_t operator()(boost::asio::ip::udp::endpoint const&) const noexcept;
	};

	template<>
	struct hash<boost::asio::ip::icmp::endpoint>
	{
		size_t operator()(boost::asio::ip::icmp::endpoint const&) const noexcept;
	};

	template<>
	struct hash<boost::asio::ip::address>
	{
		size_t operator()(boost::asio::ip::address const&) const noexcept;
	};

#if CYNG_ODBC_INSTALLED == 1
	template<>
	struct hash<SQL_TIMESTAMP_STRUCT>
	{
		size_t operator()(SQL_TIMESTAMP_STRUCT const&) const noexcept;
	};
#endif

	template<typename R, typename ...Args>
	struct hash<function<R(Args...)>>
	{
		size_t operator()(function<R(Args...)> const&) const noexcept
		{
			return 0;
		}
	};

	template<typename R, typename ...Args>
	struct equal_to<function<R(Args...)>>
	{
		using result_type = bool;
		using first_argument_type = function<R(Args...)>;
		using second_argument_type = function<R(Args...)>;

		inline bool operator()(function<R(Args...)> const& c1, function<R(Args...)> const& c2) const noexcept
		{
			return get_target_address(c1) == get_target_address(c2);
		}
	};

	template<typename R, typename ...Args>
	struct less<function<R(Args...)>>
	{
		using result_type = bool;
		using first_argument_type = function<R(Args...)>;
		using second_argument_type = function<R(Args...)>;

		inline bool operator()(function<R(Args...)> const& c1, function<R(Args...)> const& c2) const noexcept
		{
			return get_target_address(c1) < get_target_address(c2);
		}
	};

}

namespace cyng
{
	
	std::size_t hash_value(object const&);
}

#endif //	CYNG_HASH_H
