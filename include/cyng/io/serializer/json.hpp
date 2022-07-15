/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_IO_JSON_HPP
#define CYNG_IO_JSON_HPP

#include <cyng/io/io.h>
#include <cyng/obj/intrinsics/container.h>
#include <cyng/obj/intrinsics/program.h>
#include <cyng/obj/object.h>
#include <cyng/obj/intrinsics/buffer.h>
#include <cyng/obj/intrinsics/null.h>
#include <cyng/obj/intrinsics/severity.h>

#include <chrono>
#include <filesystem>

namespace cyng {
	namespace io {

		/**
		 * fallback
		 */
		template <typename T>
		struct serializer <T, JSON>
		{
			static std::size_t write(std::ostream& os, T const& v) {
				calc_size const cs(os);
				os << '"' << v << '"';
				return cs;
			}
		};

		template <>
		struct serializer <bool, JSON>
		{
			static std::size_t write(std::ostream& os, bool v);
		};

		template <>
		struct serializer <null, JSON>
		{
			static std::size_t write(std::ostream& os, null);
		};

		template <>
		struct serializer <vector_t, JSON>
		{
			static std::size_t write(std::ostream& os, vector_t const&);
		};

		template <>
		struct serializer <prg_t, JSON>
		{
			static std::size_t write(std::ostream& os, prg_t const&);
		};

		template <>
		struct serializer <tuple_t, JSON>
		{
			static std::size_t write(std::ostream& os, tuple_t const&);
		};

		template <>
		struct serializer <param_t, JSON>
		{
			static std::size_t write(std::ostream& os, param_t const&);
		};

		template <>
		struct serializer <param_map_t, JSON>
		{
			static std::size_t write(std::ostream& os, param_map_t const&);
		};

		template <>
		struct serializer <prop_t, JSON>
		{
			static std::size_t write(std::ostream& os, prop_t const&);
		};

		template <>
		struct serializer <prop_map_t, JSON>
		{
			static std::size_t write(std::ostream& os, prop_map_t const&);
		};

		template <>
		struct serializer <std::int8_t, JSON>
		{
			static std::size_t write(std::ostream& os, std::int8_t v);
		};

		template <>
		struct serializer <std::int16_t, JSON>
		{
			static std::size_t write(std::ostream& os, std::int16_t v);
		};

		template <>
		struct serializer <std::int32_t, JSON>
		{
			static std::size_t write(std::ostream& os, std::int32_t v);
		};

		template <>
		struct serializer <std::int64_t, JSON>
		{
			static std::size_t write(std::ostream& os, std::int64_t v);
		};

		template <>
		struct serializer <std::uint8_t, JSON>
		{
			static std::size_t write(std::ostream& os, std::uint8_t v);
		};

		template <>
		struct serializer <std::uint16_t, JSON>
		{
			static std::size_t write(std::ostream& os, std::uint16_t v);
		};

		template <>
		struct serializer <std::uint32_t, JSON>
		{
			static std::size_t write(std::ostream& os, std::uint32_t v);
		};

		template <>
		struct serializer <std::uint64_t, JSON>
		{
			static std::size_t write(std::ostream& os, std::uint64_t v);
		};

		template <>
		struct serializer <double, JSON>
		{
			static std::size_t write(std::ostream& os, double v);
		};

		template <>
		struct serializer <std::string, JSON>
		{
			static std::size_t write(std::ostream& os, std::string const& v);
		};

		template <>
		struct serializer <buffer_t, JSON>
		{
			static std::size_t write(std::ostream& os, buffer_t const& v);
		};

		/**
		 *  Generates a date time string that can be parsed in JavaScript with Date.parse()
		 */
		template <>
		struct serializer <std::chrono::system_clock::time_point, JSON>
		{
			static std::size_t write(std::ostream& os, std::chrono::system_clock::time_point const& v);
		};

		/**
		 *  Get rid of enclosing quotation marks 
		 */
		template <>
		struct serializer <std::filesystem::path, JSON>
		{
			static std::size_t write(std::ostream& os, std::filesystem::path const& v);
		};

		/**
		 *  Severity level without spaces
		 */
		template <>
		struct serializer <severity, JSON>
		{
			static std::size_t write(std::ostream& os, severity v);
		};


	}
}
#endif
