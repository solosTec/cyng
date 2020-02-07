/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_TRAITS_TAG_HPP
#define CYNG_TRAITS_TAG_HPP

#include <cyng/intrinsics.h>
#include <cstddef>
#include <tuple>
#include <functional>
#include <cyng/intrinsics/traits.hpp>

namespace cyng 
{
	namespace traits
	{
		/**
		 * Declare a tag list for all supported types.
		 * Note that void is not a valid tuple element.
		 *
		 * Consider using Boost.Multiprecision
		 */
		using tag_t = std::tuple<
		null,
		
		bool,
		char,
		float,
		double,
		long double,
		const char*,
		std::uint8_t, 
		std::uint16_t, 
		std::uint32_t, 
		std::uint64_t, 
		std::int8_t, 
		std::int16_t, 
		std::int32_t, 
		std::int64_t,
		std::string,
		std::chrono::system_clock::time_point,
		std::chrono::nanoseconds,
		std::chrono::microseconds,
		std::chrono::milliseconds,
		std::chrono::seconds,
		std::chrono::minutes,
		std::chrono::hours,
		
		chrono::dbl_time_point,
		version,
		revision,
		code,
		label,
		logging::severity,
		buffer_t,
		mac48,
		mac64,
		color_8,
		color_16,
		
		crypto::digest_md5,
		crypto::digest_sha1,
		crypto::digest_sha256,
		crypto::digest_sha512,

		crypto::aes_128_key,
		crypto::aes_192_key,
		crypto::aes_256_key,

		object, 		//	embedded object - don't do this
		
		tuple_t,
		vector_t,
		set_t,

		attr_map_t,		//	std::map<std::size_t, object>;
		attr_t,			//	std::pair<std::size_t, object>;
		param_map_t,	//	std::map<std::string, object>;
		param_t,		//	std::pair<std::string, object>;
		
		lockable,
		
		boost::system::error_code,
		boost::uuids::uuid,
		boost::filesystem::path,
		boost::asio::ip::tcp::endpoint,
		boost::asio::ip::udp::endpoint,
		boost::asio::ip::icmp::endpoint,
		boost::asio::ip::address,
#if defined(CYNG_ODBC_INSTALLED)
		SQL_TIMESTAMP_STRUCT,
#else
		dummy_SQL_TIMESTAMP_STRUCT,
#endif

		eod
		>;
		
		/**
		 * Test if type is element of a tuple
		 * 
		 * Example:
		 * @code
		 * using T = std::tuple<int, double, char>;
		 * std::cout << std::boolalpha;
		 * std::cout << tuple_contains_type<int, T>::value << std::endl;
		 * @endcode
		 */
		template <typename T, typename Tuple>
		struct has_type;

		template <typename T>
		struct has_type<T, std::tuple<>> 
		: std::false_type 
		{};

		template <typename T, typename U, typename... Ts>
		struct has_type<T, std::tuple<U, Ts...>> 
		: has_type<T, std::tuple<Ts...>> 
		{};

		template <typename T, typename... Ts>
		struct has_type<T, std::tuple<T, Ts...>> 
		: std::true_type 
		{};


		/**
		 * Calculate the index of a specified type in the tag tuple:
		 * 
		 * Example:
		 * @code
		 * typedef std::tuple<int, char*, int, long> foo_t;
		 * std::cout << "index<int, foo_t> = " << index<int, foo_t>::value << std::endl;
		 * @endcode
		 */
		template <class T, class Tuple>
		struct index;

		template <class T, class... Types>
		struct index<T, std::tuple<T, Types...>> 
		{
			static constexpr std::size_t value = 0;
		};

		template <class T, class U, class... Types>
		struct index<T, std::tuple<U, Types...>> 
		{
			static constexpr std::size_t value = 1 + index<T, std::tuple<Types...>>::value;
		};

	} // traits
	
	/**
	 * @return true if data type T is in the type list tag_t, i.e. it is a built-in type
	 */
	template <typename T>
	constexpr bool built_in_type()
	{
		return traits::has_type<T, traits::tag_t>::value;
	}

	/**
	 * @return true if tag is in the range of the built-in types
	 */
	inline bool built_in_tag(std::size_t tag)
	{
		return tag < std::tuple_size<traits::tag_t>::value;
	}

	/**
	 * @return the index of the specified type in the tag tuple
	 */
	template <typename T>
	constexpr typename std::enable_if<traits::has_type<T, traits::tag_t>::value, std::size_t>::type
	type_tag_traits()
	{
// 		using R = typename std::conditional<built_in_type<T>()
// 		, std::integral_constant<std::size_t, traits::index<T, traits::tag_t>::value>
// 		, std::integral_constant<std::size_t, std::tuple_size<traits::tag_t>::value>
// 		>::type;
// 		return R::value; 
		
		
		return traits::index<T, traits::tag_t>::value;
	}

	template <typename T>
	constexpr typename std::enable_if<!traits::has_type<T, traits::tag_t>::value, std::size_t>::type
	type_tag_traits()
	{
		//
		//	specialize type_tag<> for your custom data types
		//
		return cyng::traits::type_tag<T>::tag::value;
	}
	
	enum /*class*/ type_code : std::size_t
	{
		TC_NULL 	= type_tag_traits<null>(),

		TC_BOOL 	= type_tag_traits<bool>(),
		TC_CHAR 	= type_tag_traits<char>(),
		TC_FLOAT	= type_tag_traits<float>(),
		TC_DOUBLE	= type_tag_traits<double>(),
		TC_FLOAT80	= type_tag_traits<long double>(),	//	different sizes on different compilers
// 		const char*,
		TC_UINT8	= type_tag_traits<std::uint8_t>(), 
		TC_UINT16	= type_tag_traits<std::uint16_t>(),
		TC_UINT32	= type_tag_traits<std::uint32_t>(),
		TC_UINT64	= type_tag_traits<std::uint64_t>(),
		TC_INT8		= type_tag_traits<std::int8_t>(),
		TC_INT16	= type_tag_traits<std::int16_t>(),
		TC_INT32	= type_tag_traits<std::int32_t>(),
		TC_INT64	= type_tag_traits<std::int64_t>(),
		TC_STRING	= type_tag_traits<std::string>(),
		TC_TIME_POINT	= type_tag_traits<std::chrono::system_clock::time_point>(),
		TC_NANO_SECOND	= type_tag_traits<std::chrono::nanoseconds>(),
		TC_MICRO_SECOND	= type_tag_traits<std::chrono::microseconds>(),
		TC_MILLI_SECOND	= type_tag_traits<std::chrono::milliseconds>(),
		TC_SECOND	= type_tag_traits<std::chrono::seconds>(),
		TC_MINUTE	= type_tag_traits<std::chrono::minutes>(),
		TC_HOUR	= type_tag_traits<std::chrono::hours>(),
		
		TC_DBL_TP	= type_tag_traits<chrono::dbl_time_point>(),
		TC_VERSION	= type_tag_traits<version>(),
		TC_REVISION	= type_tag_traits<revision>(),
		TC_CODE		= type_tag_traits<code>(),
		TC_LABEL	= type_tag_traits<label>(),
		TC_SEVERITY = type_tag_traits<logging::severity>(),
		TC_BUFFER	= type_tag_traits<buffer_t>(),
		TC_MAC48	= type_tag_traits<mac48>(),
		TC_MAC64	= type_tag_traits<mac64>(),
		TC_COLOR_8	= type_tag_traits<color_8>(),
		TC_COLOR_16	= type_tag_traits<color_16>(),
		
		TC_DIGEST_MD5	= type_tag_traits<crypto::digest_md5>(),
		TC_DIGEST_SHA1	= type_tag_traits<crypto::digest_sha1>(),
		TC_DIGEST_SHA256	= type_tag_traits<crypto::digest_sha256>(),
		TC_DIGEST_SHA512	= type_tag_traits<crypto::digest_sha512>(),
		
		TC_AES128 = type_tag_traits<crypto::aes_128_key>(),
		TC_AES192 = type_tag_traits<crypto::aes_192_key>(),
		TC_AES256 = type_tag_traits<crypto::aes_256_key>(),

		TC_OBJECT	= type_tag_traits<object>(),
	
		TC_TUPLE	= type_tag_traits<tuple_t>(),
		TC_VECTOR	= type_tag_traits<vector_t>(),
		TC_SET		= type_tag_traits<set_t>(),

		TC_ATTR_MAP	= type_tag_traits<attr_map_t>(),		//	std::map<std::size_t, object>;
		TC_ATTR		= type_tag_traits<attr_t>(),			//	std::pair<std::size_t, object>;
		TC_PARAM_MAP	= type_tag_traits<param_map_t>(),	//	std::map<std::string, object>;
		TC_PARAM	= type_tag_traits<param_t>(),		//	std::pair<std::string, object>;
		
		TC_EC		= type_tag_traits<boost::system::error_code>(),
		TC_UUID		= type_tag_traits<boost::uuids::uuid>(),
		TC_FS_PATH	= type_tag_traits<boost::filesystem::path>(),

		TC_IP_TCP_ENDPOINT	= type_tag_traits<boost::asio::ip::tcp::endpoint>(),
		TC_IP_UDP_ENDPOINT = type_tag_traits<boost::asio::ip::udp::endpoint>(),
		TC_IP_ICMP_ENDPOINT = type_tag_traits<boost::asio::ip::icmp::endpoint>(),
		TC_IP_ADDRESS = type_tag_traits<boost::asio::ip::address>(),

#if defined(CYNG_ODBC_INSTALLED)
		TC_SQL_TIMESTAMP = type_tag_traits<SQL_TIMESTAMP_STRUCT>(),
#else
		TC_SQL_TIMESTAMP = type_tag_traits<dummy_SQL_TIMESTAMP_STRUCT>(),
#endif

		TC_EOD		= type_tag_traits<eod>()
	};
	
	namespace traits
	{	
		/**
		 * Calculate the data type from it's type code.
		 * Overwrite this template for custom types.
		 */
		template < std::size_t N >
		struct reverse_type
		{
			using type = typename std::tuple_element<N, traits::tag_t>::type;
		};
	}
}

#endif	//	CYNG_TRAITS_TAG_HPP

