/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_PARSE_STRING_H
#define CYNG_PARSE_STRING_H

#include <cyng/obj/intrinsics/aes_key.hpp>
#include <cyng/obj/buffer_cast.hpp>
#include <cyng/obj/util.hpp>
#include <cyng/parse/buffer.h>

#include <string>
#include <vector>
#include <chrono>

#include <boost/utility/string_view.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/asio/ip/address.hpp>

namespace cyng {
	std::vector<std::string> split(std::string str, std::string delims);
	std::vector<boost::string_view> split(boost::string_view str, boost::string_view delims);

	/**
	 * convert hex string to UUID
	 */
	[[nodiscard]]
	boost::uuids::uuid to_uuid(std::string const&);

	[[nodiscard]]
	boost::uuids::uuid to_uuid(std::string const&, boost::uuids::uuid);

	/**
	 * 2021-03-31T19:36:45+0100	- ISO-8601
	 */
	[[nodiscard]]
	std::chrono::system_clock::time_point to_tp_iso8601(std::string const&);

	/**
	 * "%Y-%m-%d %H:%M:%S" - SQLite datetime()
	 */
	[[nodiscard]]
	std::chrono::system_clock::time_point to_tp_datetime(std::string const&);

	[[nodiscard]]
	boost::asio::ip::address to_ip_address(std::string const&);

	template <std::size_t N>
	[[nodiscard]]
	auto to_aes_key(std::string const& str) -> aes_key<N> {

		BOOST_ASSERT_MSG(str.size() == N / 4, "invalid AES key format");

		auto const buffer = hex_to_buffer(str);
		return make_aes_key<N>(buffer);

	}

	namespace {

		template <typename T, int BASE>
		struct string_policy {
			static T cast(std::string const& str) {
				//static_assert(false, "not a numeric type");
				return T();
			}
		};
		template <int BASE>
		struct string_policy<std::uint8_t, BASE> {
			static std::uint8_t cast(std::string const& str) {
				try { 
					return static_cast<std::uint8_t>(stoul(str, 0, BASE)); 
				}
				catch (std::exception const&) {}
				return 0;
			}
		};
		template <int BASE>
		struct string_policy<std::uint16_t, BASE> {
			static std::uint16_t cast(std::string const& str) {
				try { 
					return static_cast<std::uint16_t>(stoul(str, 0, BASE)); 
				}
				catch (std::exception const&) {}
				return 0;
			}
		};
		template <int BASE>
		struct string_policy<std::uint32_t, BASE> {
			static std::uint32_t cast(std::string const& str) {
				try { 
					return static_cast<std::uint32_t>(stoul(str, 0, BASE)); 
				}
				catch (std::exception const&) {}
				return 0;
			}
		};
		template <int BASE>
		struct string_policy<std::uint64_t, BASE> {
			static std::uint64_t cast(std::string const& str) {
				try {
					return static_cast<std::uint64_t>(stoull(str, 0, BASE));
				}
				catch (std::exception const&) {}
				return 0;
			}
		};
		template <int BASE>
		struct string_policy<std::int8_t, BASE> {
			static std::int8_t cast(std::string const& str) {
				try {
					return static_cast<std::int8_t>(stoi(str, 0, BASE));
				}
				catch (std::exception const&) {}
				return 0;
			}
		};
		template <int BASE>
		struct string_policy<std::int16_t, BASE> {
			static std::int16_t cast(std::string const& str) {
				try {
					return static_cast<std::int16_t>(stoi(str, 0, BASE));
				}
				catch (std::exception const&) {}
				return 0;
			}
		};
		template <int BASE>
		struct string_policy<std::int32_t, BASE> {
			static std::int32_t cast(std::string const& str) {
				try {
					return static_cast<std::int32_t>(stol(str, 0, BASE));
				}
				catch (std::exception const&) {}
				return 0;
			}
		};
		template <int BASE>
		struct string_policy<std::int64_t, BASE> {
			static std::int64_t cast(std::string const& str) {
				try {
					return static_cast<std::int64_t>(stoll(str, 0, BASE));
				}
				catch (std::exception const&) {}
				return 0;
			}
		};
		template <int BASE>
		struct string_policy<float, BASE> {
			static float cast(std::string const& str) {
				try {
					return stof(str, 0);
				}
				catch (std::exception const&) {}
				return 0;
			}
		};
		template <int BASE>
		struct string_policy<double, BASE> {
			static double cast(std::string const& str) {
				try { 
					return stod(str, 0); 
				}
				catch (std::exception const&) {}
				return 0;
			}
		};
		template <int BASE>
		struct string_policy<long double, BASE> {
			static long double cast(std::string const& str) {
				try {
					return stold(str, 0);
				}
				catch (std::exception const&) {}
				return 0;
			}
		};

	}

	template <typename T, int BASE = 10>
	[[nodiscard]]
	T to_numeric(std::string const& str) {
		return string_policy<T, BASE>::cast(str);
	}

}
#endif

