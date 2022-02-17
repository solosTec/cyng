/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_IO_OSTREAM_H
#define CYNG_IO_OSTREAM_H

#include <string>
#include <chrono>
#include <filesystem>
#include <sstream>

#include <boost/uuid/uuid.hpp>
#include <boost/system/error_code.hpp>

#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/ip/icmp.hpp>

#include <cyng/obj/intrinsics.h>
#include <cyng/obj/object.h>

#include <ostream>
#include <iomanip>
#include <boost/io/ios_state.hpp>

namespace cyng {

	//
	//	catch the stream operators from namespace cyng
	//using cyng::operator<<;

	std::ostream& operator<<(std::ostream& os, null const&);
	std::ostream& operator<<(std::ostream& os, eod const&);

	std::ostream& operator<<(std::ostream& os, version const&);
	std::ostream& operator<<(std::ostream& os, revision const&);

	std::ostream& operator<<(std::ostream& os, op);
	std::ostream& operator<<(std::ostream& os, severity);
	/**
	 *  format is xx:xx:xx:xx:xx:xx (17 bytes)
	 */
	std::ostream& operator<<(std::ostream& os, mac48 const&);
	std::ostream& operator<<(std::ostream& os, mac64 const&);

	std::ostream& operator<<(std::ostream& os, pid const&);

	std::ostream& operator<<(std::ostream& os, obis const&);
	std::ostream& operator<<(std::ostream& os, obis_path_t const&);
	std::ostream& operator<<(std::ostream& os, edis const&);

	template <std::size_t N>
	std::ostream& operator<<(std::ostream& os, digest<N> const& d) {

		//	store and reset stream state
		boost::io::ios_flags_saver  ifs(os);

		os
			<< std::hex
			<< std::setfill('0')
			;

		//
		// 	format xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
		//	32/40/64/128 characters

		for (auto const c : d.digest_) {
			os
				<< std::setw(2)
				<< (+c & 0xFF)
				;
		}

		return os;
	}

	template <std::size_t N>
	std::ostream& operator<<(std::ostream& os, aes_key<N> const& key) {

		//	store and reset stream state
		boost::io::ios_flags_saver  ifs(os);

		os
			<< std::hex
			<< std::setfill('0')
			;

		for (auto const c : key.key_) {
			os
				<< std::setw(2)
				<< (+c & 0xFF)	//	promote to integer
				;
		}

		return os;
	}

	/**
	 * Serialize a buffer with escaped non-printable ASCII characters
	 */
	std::ostream& operator<<(std::ostream& os, buffer_t const&);

	/**
	 * Write a timespan in hh::mm::ss.ffff format.
	 *
	 * @tparam R an arithmetic type representing the number of ticks
	 * @tparam P a std::ratio representing the tick period (i.e. the number of seconds per tick)
	 */
	template <typename R, typename P>
	std::ostream& operator<<(std::ostream& os, std::chrono::duration<R, P> const& v)
	{
		const std::chrono::hours   h = std::chrono::duration_cast<std::chrono::hours>(v);
		const std::chrono::minutes  m = std::chrono::duration_cast<std::chrono::minutes>(v - h);
		const std::chrono::duration<float> s = v - h - m;

		boost::io::ios_flags_saver  ifs(os);

		os
			<< std::setfill('0')
			<< std::setw(2)
			<< h.count()
			<< ':'
			<< std::setw(2)
			<< (m.count() % 60ULL)	//	minutes
			<< ':'
			//	add fractional part of seconds
#if defined(_MSC_VER) && (_MSC_VER < 1700)
			<< std::defaultfloat	//	unknown modifier for GNU libstdc++ version 20150327
#else
			<< std::setprecision(6)
			<< std::fixed
#endif
			<< s.count()
			;
		return os;
	}

	/**
	 * Write the time stamp in the following format:
	 *
	 * @code
	 * yyyy-mm-ddThh:mm:ss+NNNN
	 * @endcode
	 *
	 * All elements of fixed with and padded with zeros (0) if required
	 */
	std::ostream& operator<<(std::ostream& os, std::chrono::system_clock::time_point const&);
	std::ostream& operator<<(std::ostream& os, std::chrono::steady_clock::time_point const&);

	std::ostream& operator<<(std::ostream& os, boost::uuids::uuid const&);

	std::ostream& operator<<(std::ostream& os, raw const&);

	//
	//	container
	//
	std::ostream& operator<<(std::ostream& os, tuple_t const&);
	std::ostream& operator<<(std::ostream& os, vector_t const&);
	std::ostream& operator<<(std::ostream& os, deque_t const&);

	//
	//	program
	//
	std::ostream& operator<<(std::ostream& os, prg_t const&);

	/**
	 * attributes are sorted by an index
	 */
	std::ostream& operator<<(std::ostream& os, attr_map_t const&);
	std::ostream& operator<<(std::ostream& os, attr_t const&);

	/**
	 * prameters are sorted by an identifier (string)
	 */
	std::ostream& operator<<(std::ostream& os, param_map_t const&);
	std::ostream& operator<<(std::ostream& os, param_t const&);

	/**
	 * properties are sorted by an code (obis)
	 */
	std::ostream& operator<<(std::ostream& os, prop_map_t const&);
	std::ostream& operator<<(std::ostream& os, prop_t const&);

	template <typename T>
	std::ostream& operator<<(std::ostream& os, color<T> const& v)
	{
		boost::io::ios_flags_saver  ifs(os);

		os
			<< "#"
			<< std::setfill('0')
			<< std::hex
			<< std::setw(2 * sizeof(T))
			<< +v.red()
			<< std::setw(2 * sizeof(T))
			<< +v.green()
			<< std::setw(2 * sizeof(T))
			<< +v.blue()
			;
		return os;
	}

	/**
	 * object serialization
	 */
	std::ostream& operator<<(std::ostream& os, object const&);

	/**
	 * generic to_string() convert
	 */
	template <typename T>
	std::string to_string(T const& v) {
		std::stringstream ss;
		ss << v;
		return ss.str();
	}

	/**
	 * Convert an arbitrary list of objects into a path
	 * separated by a delimiter symbol.
	 * see https://en.cppreference.com/w/cpp/utility/integer_sequence
	 */
	template<typename Head, typename... Args>
	std::string to_path(char delim, Head&& v, Args&&... args) {
		std::stringstream ss;
		ss << v;
		((ss << delim << args ), ...);
		return ss.str();
	}
}
#endif

