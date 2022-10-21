/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_IO_CPP_HPP
#define CYNG_IO_CPP_HPP

#include <cyng/io/io.h>
#include <cyng/obj/intrinsics.h>
#include <cyng/obj/tag.hpp>

namespace cyng {
	namespace io {

		/**
		 * fallback
		 */
		template <typename T>
		struct serializer <T, CPP>
		{
			static std::size_t write(std::ostream& os, T const& v) {
				calc_size const cs(os);
				os << '"' << v << '"';
				return cs;
			}
		};

		template <>
		struct serializer <null, CPP>
		{
			static std::size_t write(std::ostream& os, null n);
		};

		template <>
		struct serializer <eod, CPP>
		{
			static std::size_t write(std::ostream& os, eod e);
		};

		template <>
		struct serializer <bool, CPP>
		{
			static std::size_t write(std::ostream& os, bool v);
		};


		//template <>
		//struct serializer <char, CPP>
		//{
		//	static std::size_t write(std::ostream& os, char v);
		//};

		template <>
		struct serializer <std::int8_t, CPP>
		{
			static std::size_t write(std::ostream& os, std::int8_t v);
		};

		template <>
		struct serializer <std::int16_t, CPP>
		{
			static std::size_t write(std::ostream& os, std::int16_t v);
		};

		template <>
		struct serializer <std::int32_t, CPP>
		{
			static std::size_t write(std::ostream& os, std::int32_t v);
		};

		template <>
		struct serializer <std::int64_t, CPP>
		{
			static std::size_t write(std::ostream& os, std::int64_t v);
		};

		template <>
		struct serializer <std::uint8_t, CPP>
		{
			static std::size_t write(std::ostream& os, std::uint8_t v);
		};

		template <>
		struct serializer <std::uint16_t, CPP>
		{
			static std::size_t write(std::ostream& os, std::uint16_t v);
		};

		template <>
		struct serializer <std::uint32_t, CPP>
		{
			static std::size_t write(std::ostream& os, std::uint32_t v);
		};

		template <>
		struct serializer <std::uint64_t, CPP>
		{
			static std::size_t write(std::ostream& os, std::uint64_t v);
		};

		template <>
		struct serializer <double, CPP>
		{
			static std::size_t write(std::ostream& os, double v);
		};

		template <>
		struct serializer <std::string, CPP> 
		{
			static std::size_t write(std::ostream& os, std::string const& str);
		};

		template <>
		struct serializer <std::filesystem::path, CPP>
		{
			static std::size_t write(std::ostream& os, std::filesystem::path const&);
		};

		//template<typename T>
		//struct serializer <boost::asio::ip::basic_endpoint<T>, CPP>
		//{
		//	static std::size_t write(std::ostream& os, boost::asio::ip::basic_endpoint<T> const& ep) {

		//		auto const address = ep.address().to_string();
		//		auto const port = ep.port();

		//		//
		//		//	address:port
		//		//
		//		os
		//			<< address
		//			<< ':'
		//			<< port
		//			;

		//		return address.size() + sizeof(port) + 1;
		//	}
		//};

		template <>
		struct serializer <severity, CPP>
		{
			static std::size_t write(std::ostream& os, severity s);
		};

		template <>
		struct serializer <boost::uuids::uuid, CPP>
		{
			static std::size_t write(std::ostream& os, boost::uuids::uuid const& v);
		};

		template <>
		struct serializer <vector_t, CPP>
		{
			static std::size_t write(std::ostream& o, vector_t const& v);
		};

		template <>
		struct serializer <tuple_t, CPP>
		{
			static std::size_t write(std::ostream& o, tuple_t const& v);
		};

		//template <>
		//struct serializer <deque_t, CPP>
		//{
		//	static std::size_t write(std::ostream& o, deque_t const& v);
		//};

		template <>
		struct serializer <param_t, CPP>
		{
			static std::size_t write(std::ostream& o, param_t const& v);
		};

		//template <>
		//struct serializer <attr_t, CPP>
		//{
		//	static std::size_t write(std::ostream& o, attr_t const& v);
		//};

		//template <>
		//struct serializer <param_map_t, CPP>
		//{
		//	static std::size_t write(std::ostream& o, param_map_t const& v);
		//};

		//template <>
		//struct serializer <attr_map_t, CPP>
		//{
		//	static std::size_t write(std::ostream& o, attr_map_t const& v);
		//};

		///**
		// * Write a timespan in hh::mm::ss.ffff format.
		// *
		// * @tparam R an arithmetic type representing the number of ticks
		// * @tparam P a std::ratio representing the tick period (i.e. the number of seconds per tick)
		// */
		//template <typename R, typename P>
		//struct serializer <std::chrono::duration<R, P>, CPP>
		//{
		//	using type = std::chrono::duration<R, P>;
		//	static std::size_t write(std::ostream& o, std::chrono::duration<R, P> const& v) {
		//		//	ToDO:
		//		return 0;
		//	}
		//};

		//template <>
		//struct serializer <std::chrono::system_clock::time_point, CPP>
		//{
		//	static std::size_t write(std::ostream& o, std::chrono::system_clock::time_point const&);
		//};

	}
}
#endif
