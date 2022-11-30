/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_TAG_HPP
#define CYNG_OBJ_TAG_HPP

#include <cyng/meta.hpp>
#include <cyng/obj/intrinsics.h>

namespace cyng {
    namespace traits {

        using tag_t = std::tuple<

            null, //	avoid hussle with void

            bool,
            char,
            float,
            double,
            long double,

            std::uint8_t,
            std::uint16_t,
            std::uint32_t,
            std::uint64_t,
            std::int8_t,
            std::int16_t,
            std::int32_t,
            std::int64_t,

            std::string,
            std::filesystem::path,
            // std::filesystem::file_status,

            std::chrono::system_clock::time_point,
            std::chrono::nanoseconds,
            std::chrono::microseconds,
            std::chrono::milliseconds,
            std::chrono::seconds,
            std::chrono::minutes,
            std::chrono::hours,

            buffer_t,
            version,
            revision,
            op,
            severity,
            mac48,
            mac64,
            pid,
            obis,
            obis_path_t,
            edis,
            color_8,
            color_16,

            crypto::digest_md5,
            crypto::digest_sha1,
            crypto::digest_sha256,
            crypto::digest_sha512,

            crypto::aes_128_key,
            crypto::aes_192_key,
            crypto::aes_256_key,

            object, //	object is also part of the type system
            raw,    //	unparsed object
            date,

            tuple_t,
            vector_t,
            deque_t,

            attr_map_t,  //	std::map<std::size_t, object>;
            attr_t,      //	std::pair<std::size_t, object>;
            param_map_t, //	std::map<std::string, object>;
            param_t,     //	std::pair<std::string, object>;
            prop_map_t,  //	std::map<obis, object>;
            prop_t,      //	std::pair<obis, object>;

            prg_t, //	std::deque<buffer_t>

            boost::system::error_code,
            boost::uuids::uuid,

            boost::asio::ip::address,
            boost::asio::ip::tcp::endpoint,
            boost::asio::ip::udp::endpoint,
            boost::asio::ip::icmp::endpoint,

            eod //	last entry

            >;

        /**
         * Calculate the data type from it's type code.
         * Overwrite this template for custom types.
         */
        template <std::size_t N> struct reverse_type {
            using type = typename std::tuple_element<N, tag_t>::type;
        };

        /**
         * array of type names (has the same size as the type tuple above)
         */
        constexpr char const *names[std::tuple_size<tag_t>::value] = {

            "null",

            "bool",
            "char",
            "f",  //	float
            "d",  //	double
            "ld", //	long double

            "u8",  //	std::uint8_t
            "u16", // 	std::uint16_t
            "u32", //	uint32_t
            "u64", //	uint64_t
            "i8",  // 	int8_t
            "i16", //	int16_t
            "i32", //	int32_t
            "i64", //	int64_t

            "s",       //	string
            "fs:path", //	boost::filesystem::path

            "chrono:tp",  //	chrono::system_clock::time_point
            "chrono:ns",  //	chrono::nanoseconds
            "chrono:us",  //	chrono::microseconds
            "chrono:ms",  //	chrono::milliseconds
            "chrono:sec", //	chrono::seconds
            "chrono:min", //	chrono::minutes
            "chrono:h",   //	chrono::hours

            "binary",   //	buffer_t
            "v",        //	version
            "r",        //	revision
            "op",       //	op code
            "severity", //	log level
            "mac48",
            "mac64",
            "pid",
            "obis",
            "obis:path",
            "edis",
            "color:8",
            "color:16",

            "crypto:MD5",
            "crypto:SHA1",
            "crypto:SHA256",
            "crypto:SHA512",

            "crypto::AES128",
            "crypto::AES192",
            "crypto::AES256",

            "obj", //	object
            "raw", //	unparsed object
            "date",

            "tpl",   //	tuple_t
            "vec",   //	vector_t
            "deque", //	deque_t

            "amap",  //	attr_map_t - std::map<std::size_t, object>;
            "attr",  //	attr_t - std::pair<std::size_t, object>;
            "pmap",  //	param_map_t - std::map<std::string, object>;
            "param", //	param_t - std::pair<std::string, object>;
            "omap",  //	prop_map_t - std::map<obis, object>;
            "prop",  //	prop_t - std::pair<obis, object>;

            "prg", //	prg_t - std::deque<buffer_t>

            "ec",   //	boost::system::error_code
            "uuid", //	boost::uuids::uuid

            "ip:address", //	boost::asio::ip::address
            "ip:tcp:ep",  //	boost::asio::ip::tcp::endpoint
            "ip:udp:ep",  //	boost::asio::ip::udp::endpoint
            "ip:icmp:ep", //	boost::asio::ip::icmp::endpoint

            "eod" //	end-of-data
        };

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
        template <typename T, typename Tuple> struct has_type;

        template <typename T> struct has_type<T, std::tuple<>> : std::false_type {};

        template <typename T, typename U, typename... Ts>
        struct has_type<T, std::tuple<U, Ts...>> : has_type<T, std::tuple<Ts...>> {};

        template <typename T, typename... Ts> struct has_type<T, std::tuple<T, Ts...>> : std::true_type {};
    } // namespace traits

    /**
     * @return true if data type T is in the type list tag_t, i.e. it is a built-in type
     */
    template <typename T> constexpr bool built_in_type() noexcept { return traits::has_type<T, traits::tag_t>::value; }

    /**
     * @return the index of the specified type in the type tuple
     */
    template <typename T>
    constexpr typename std::enable_if<traits::has_type<T, traits::tag_t>::value, std::uint16_t>::type type_tag_traits() noexcept {
        return tmp::index<T, traits::tag_t>::value;
    }

    template <typename T>
    constexpr typename std::enable_if<!traits::has_type<T, traits::tag_t>::value, std::uint16_t>::type type_tag_traits() noexcept {
        //
        //	specialize type_tag<> for your custom data types
        //
        return -1; //	TC_EXTRINSIC == 61
                   // return cyng::traits::type_tag<T>::tag::value;
    }

    template <typename T> constexpr char const *intrinsic_name() {
        return (type_tag_traits<T>() < std::tuple_size<traits::tag_t>::value) ? traits::names[type_tag_traits<T>()] : "custom";
    }

    /**
     * The value of each enum is the index in the tag_t tuple and will be computed at compile time.
     * This guaranties constistent values. The downside is, that this anum cannot be part of the internal
     * type system, since enums cannot be forwarded.
     */
    enum type_code : std::uint16_t {
        TC_NULL = type_tag_traits<null>(),

        TC_BOOL = type_tag_traits<bool>(),
        TC_CHAR = type_tag_traits<char>(),
        TC_FLOAT = type_tag_traits<float>(),
        TC_DOUBLE = type_tag_traits<double>(),
        TC_FLOAT80 = type_tag_traits<long double>(), //	different sizes on different compilers

        TC_UINT8 = type_tag_traits<std::uint8_t>(),
        TC_UINT16 = type_tag_traits<std::uint16_t>(),
        TC_UINT32 = type_tag_traits<std::uint32_t>(),
        TC_UINT64 = type_tag_traits<std::uint64_t>(),
        TC_INT8 = type_tag_traits<std::int8_t>(),
        TC_INT16 = type_tag_traits<std::int16_t>(),
        TC_INT32 = type_tag_traits<std::int32_t>(),
        TC_INT64 = type_tag_traits<std::int64_t>(),

        TC_STRING = type_tag_traits<std::string>(),
        TC_FS_PATH = type_tag_traits<std::filesystem::path>(),

        TC_TIME_POINT = type_tag_traits<std::chrono::system_clock::time_point>(),
        TC_NANO_SECOND = type_tag_traits<std::chrono::nanoseconds>(),
        TC_MICRO_SECOND = type_tag_traits<std::chrono::microseconds>(),
        TC_MILLI_SECOND = type_tag_traits<std::chrono::milliseconds>(),
        TC_SECOND = type_tag_traits<std::chrono::seconds>(),
        TC_MINUTE = type_tag_traits<std::chrono::minutes>(),
        TC_HOUR = type_tag_traits<std::chrono::hours>(),

        TC_BUFFER = type_tag_traits<buffer_t>(),
        TC_VERSION = type_tag_traits<version>(),
        TC_REVISION = type_tag_traits<revision>(),
        TC_OP = type_tag_traits<op>(),
        TC_SEVERITY = type_tag_traits<severity>(),
        TC_MAC48 = type_tag_traits<mac48>(),
        TC_MAC64 = type_tag_traits<mac64>(),
        TC_PID = type_tag_traits<pid>(),
        TC_OBIS = type_tag_traits<obis>(),
        TC_OBISPATH = type_tag_traits<obis_path_t>(),
        TC_EDIS = type_tag_traits<edis>(),
        TC_COLOR_8 = type_tag_traits<color_8>(),
        TC_COLOR_16 = type_tag_traits<color_16>(),

        TC_DIGEST_MD5 = type_tag_traits<crypto::digest_md5>(),
        TC_DIGEST_SHA1 = type_tag_traits<crypto::digest_sha1>(),
        TC_DIGEST_SHA256 = type_tag_traits<crypto::digest_sha256>(),
        TC_DIGEST_SHA512 = type_tag_traits<crypto::digest_sha512>(),

        TC_AES128 = type_tag_traits<crypto::aes_128_key>(),
        TC_AES192 = type_tag_traits<crypto::aes_192_key>(),
        TC_AES256 = type_tag_traits<crypto::aes_256_key>(),

        TC_OBJECT = type_tag_traits<object>(),
        TC_RAW = type_tag_traits<raw>(),
        TC_DATE = type_tag_traits<date>(),

        TC_TUPLE = type_tag_traits<tuple_t>(),
        TC_VECTOR = type_tag_traits<vector_t>(),
        TC_DEQUE = type_tag_traits<deque_t>(),

        TC_ATTR_MAP = type_tag_traits<attr_map_t>(),   //	std::map<std::size_t, object>;
        TC_ATTR = type_tag_traits<attr_t>(),           //	std::pair<std::size_t, object>;
        TC_PARAM_MAP = type_tag_traits<param_map_t>(), //	std::map<std::string, object>;
        TC_PARAM = type_tag_traits<param_t>(),         //	std::pair<std::string, object>;
        TC_PROP_MAP = type_tag_traits<prop_map_t>(),   //	std::map<obis, object>;
        TC_PROP = type_tag_traits<prop_t>(),           //	std::pair<obis, object>;
        TC_PRG = type_tag_traits<prg_t>(),             //	std::deque<buffer_t>

        TC_EC = type_tag_traits<boost::system::error_code>(),
        TC_UUID = type_tag_traits<boost::uuids::uuid>(),

        TC_IP_ADDRESS = type_tag_traits<boost::asio::ip::address>(),
        TC_IP_TCP_ENDPOINT = type_tag_traits<boost::asio::ip::tcp::endpoint>(),
        TC_IP_UDP_ENDPOINT = type_tag_traits<boost::asio::ip::udp::endpoint>(),
        TC_IP_ICMP_ENDPOINT = type_tag_traits<boost::asio::ip::icmp::endpoint>(),

        TC_EOD = type_tag_traits<eod>(),

        TC_EXTRINSIC = type_tag_traits<eod>() + 1, //	not intrinsic

    };

    /**
     * Simple wrapper for the
     * @return the type_code type_tag_traits<>() function.
     */
    template <typename T> constexpr type_code get_type_code() { return static_cast<type_code>(type_tag_traits<T>()); }

    /**
     * Search for a matching type name. The search is case sensitive.
     */
    type_code type_code_by_name(std::string);

    /**
     * @return true if type code name exists
     */
    bool type_code_exists(std::string);

    /**
     * @return the type name from the traits::names table
     */
    constexpr char const *intrinsic_name_by_type_code(type_code tc) { return (tc < TC_EOD) ? traits::names[tc] : ""; }

    /**
     * @return true if data type is a container
     */
    constexpr bool is_container(type_code tc) {
        //	c++14 required
        switch (tc) {
        case TC_TUPLE:
        case TC_VECTOR:
        case TC_DEQUE:

        case TC_ATTR_MAP:
        case TC_ATTR:
        case TC_PARAM_MAP:
        case TC_PARAM:
        case TC_PROP_MAP:
        case TC_PROP:
        case TC_PRG: return true;
        default: break;
        }
        return false;
    }

    //
    //	provide support for the "raw" data type
    //

    /**
     * create a raw object from the type information
     */
    template <typename T> raw make_raw(std::string const &lit) noexcept { return raw(lit, get_type_code<T>()); }

} // namespace cyng

#include <boost/functional/hash.hpp>
#include <functional>

namespace std {

    //
    //	inject hash calculation into std namespace
    //
    template <typename R, typename P> class hash<std::chrono::duration<R, P>> {
      public:
        size_t operator()(std::chrono::duration<R, P> const &d) const noexcept { return d.count(); }
    };

    template <> class hash<boost::system::error_code> {
      public:
        size_t operator()(boost::system::error_code const &ec) const noexcept;
    };

    /**
     * Since MSVC version 19.32 STL defines this specialization of std::hash<>.
     */
#if defined(_MSC_VER) && (_MSC_VER < 1931) || defined(__GNUC__) && (__GNUC__ < 12)
    // #if defined(_MSC_VER) && (_MSC_VER < 1931) || defined(__GNUC__) && (__cplusplus < 201703L)
    template <> class hash<std::filesystem::path> {
      public:
        size_t operator()(std::filesystem::path const &p) const noexcept;
    };
#endif

    template <> class hash<boost::uuids::uuid> {
      public:
        size_t operator()(boost::uuids::uuid const &tag) const noexcept;
    };

    template <> struct hash<chrono::system_clock::time_point> {
        /**
         * return hash of duration
         */
        size_t operator()(chrono::system_clock::time_point const &tp) const noexcept;
    };

    /**
     * Partial specialized for alle endpoint types
     * Since Boost 1.76 Asio defines it own hash algorithm for std::hash<>().
     */
#if !defined(BOOST_ASIO_HAS_STD_HASH)
    template <typename T> struct hash<boost::asio::ip::basic_endpoint<T>> {
        using ep_type = boost::asio::ip::basic_endpoint<T>;
        size_t operator()(ep_type const &ep) const noexcept {
            std::size_t seed = 0;
            boost::hash_combine(seed, ep.address().to_string());
            boost::hash_combine(seed, ep.port());
            return seed;
        }
    };

    template <> struct hash<boost::asio::ip::address> {
        size_t operator()(boost::asio::ip::address const &addr) const noexcept;
    };
#endif
} // namespace std

#endif
