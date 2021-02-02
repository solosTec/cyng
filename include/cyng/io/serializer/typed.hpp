/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_IO_TYPED_HPP
#define DOCC_IO_TYPED_HPP

#include <cyng/io/io.h>
#include <cyng/obj/core/type.hpp>
#include <cyng/obj/intrinsics/version.h>

namespace docscript {
	namespace io {

		template <typename T>
		struct serializer <T, TYPED>
		{
			static const type<T> type_;

			static std::size_t write(std::ostream& os, T const& v) {
				os << v << ':' << type_.type_name();
				return 0;
			}
		};

		/**
		 * Initialize static data member type_
		 */
		template <typename T>
		type<T> const  serializer <T, TYPED>::type_;


		template <>
		struct serializer <bool, TYPED>
		{
			static std::size_t write(std::ostream& os, bool v);
		};

		template <>
		struct serializer <version, TYPED>
		{
			static std::size_t write(std::ostream& os, version const& v);
		};

		template <>
		struct serializer <revision, TYPED>
		{
			static std::size_t write(std::ostream& os, revision const& v);
		};

		template <>
		struct serializer <std::int8_t, TYPED>
		{
			static std::size_t write(std::ostream& os, std::int8_t v);
		};

		template <>
		struct serializer <std::int16_t, TYPED>
		{
			static std::size_t write(std::ostream& os, std::int16_t v);
		};

		template <>
		struct serializer <std::int32_t, TYPED>
		{
			static std::size_t write(std::ostream& os, std::int32_t v);
		};

		template <>
		struct serializer <std::int64_t, TYPED>
		{
			static std::size_t write(std::ostream& os, std::int64_t v);
		};

		template <>
		struct serializer <std::uint8_t, TYPED>
		{
			static std::size_t write(std::ostream& os, std::uint8_t v);
		};

		template <>
		struct serializer <std::uint16_t, TYPED>
		{
			static std::size_t write(std::ostream& os, std::uint16_t v);
		};

		template <>
		struct serializer <std::uint32_t, TYPED>
		{
			static std::size_t write(std::ostream& os, std::uint32_t v);
		};

		template <>
		struct serializer <std::uint64_t, TYPED>
		{
			static std::size_t write(std::ostream& os, std::uint64_t v);
		};

		template <>
		struct serializer <float, TYPED>
		{
			static std::size_t write(std::ostream& os, float v);
		};

		template <>
		struct serializer <double, TYPED>
		{
			static std::size_t write(std::ostream& os, double v);
		};

		template <>
		struct serializer <std::chrono::system_clock::time_point, TYPED>
		{
			static std::size_t write(std::ostream& os, std::chrono::system_clock::time_point const& v);
		};

		template <>
		struct serializer <std::string, TYPED>
		{
			static std::size_t write(std::ostream& os, std::string const& v);
		};

		template <>
		struct serializer <std::filesystem::path, TYPED>
		{
			static std::size_t write(std::ostream& os, std::filesystem::path const& v);
		};

		template <>
		struct serializer <null, TYPED>
		{
			static std::size_t write(std::ostream& os, null);
		};

		template <>
		struct serializer <eod, TYPED>
		{
			static std::size_t write(std::ostream& os, eod);
		};

		template <>
		struct serializer <op, TYPED>
		{
			static std::size_t write(std::ostream& os, op);
		};

		template <>
		struct serializer <severity, TYPED>
		{
			static std::size_t write(std::ostream& os, severity);
		};

	}
}
#endif
