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
#include <cyng/obj/object.h>
#include <cyng/obj/intrinsics/buffer.h>
#include <cyng/obj/intrinsics/null.h>

namespace cyng {
	namespace io {

		template <typename T>
		struct serializer <T, JSON>
		{
			static std::size_t write(std::ostream& os, T const& v) {
				auto const pos = os.tellp();
				os << '"' << v << '"';
				return os.tellp() - pos;
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

	}
}
#endif