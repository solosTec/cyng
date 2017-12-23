/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_IO_SERIALIZE_JSON_HPP
#define CYNG_IO_SERIALIZE_JSON_HPP

#include <cyng/io.h>
#include <cyng/intrinsics.h>
#include <cyng/io/io_chrono.hpp>
#include <iostream>
#include <iomanip>
#include <boost/uuid/uuid_io.hpp>

namespace cyng 
{	
	namespace io
	{

		template <typename T>
		struct serializer<T, SERIALIZE_JSON> 
		{		
			static std::ostream& write(std::ostream& os, T const& v)
			{
				os << '"' << v << '"';
// 				os << "JSON not implemented yet";
				return os;
			}
		};
		
		template <>
		struct serializer <param_t, SERIALIZE_JSON>
		{
			static std::ostream& write(std::ostream& os, param_t const& v);
		};
		
		template <>
		struct serializer <attr_t, SERIALIZE_JSON>
		{
			static std::ostream& write(std::ostream& os, attr_t const& v);
		};

		template <>
		struct serializer <tuple_t, SERIALIZE_JSON>
		{
			static std::ostream& write(std::ostream& os, tuple_t const& v);
		};

		template <>
		struct serializer <vector_t, SERIALIZE_JSON>
		{
			static std::ostream& write(std::ostream& os, vector_t const& v);
		};

		template <>
		struct serializer <set_t, SERIALIZE_JSON>
		{
			static std::ostream& write(std::ostream& os, set_t const& v);
		};
		
		template <>
		struct serializer <attr_map_t, SERIALIZE_JSON>
		{
			static std::ostream& write(std::ostream& os, attr_map_t const& v);
		};

		template <>
		struct serializer <param_map_t, SERIALIZE_JSON>
		{
			static std::ostream& write(std::ostream& os, param_map_t const& v);
		};

		template <>
		struct serializer <std::string, SERIALIZE_JSON>
		{
			static std::ostream& write(std::ostream& os, std::string const& v);
		};

		template <>
		struct serializer <bool, SERIALIZE_JSON>
		{
			static std::ostream& write(std::ostream& os, bool v);
		};
		
		template <>
		struct serializer <code, SERIALIZE_JSON>
		{
			static std::ostream& write(std::ostream& os, code v);
		};
		
		template <>
		struct serializer <boost::uuids::uuid, SERIALIZE_JSON>
		{
			static std::ostream& write(std::ostream& os, boost::uuids::uuid v);
		};
		
		template <>
		struct serializer <buffer_t, SERIALIZE_JSON>
		{
			static std::ostream& write(std::ostream& os, buffer_t const& v);
		};
		
		template <>
		struct serializer <color_8, SERIALIZE_JSON>
		{
			static std::ostream& write(std::ostream& os, color_8 const& v);
		};
		
		template <>
		struct serializer <color_16, SERIALIZE_JSON>
		{
			static std::ostream& write(std::ostream& os, color_16 const& v);
		};
		
		template <>
		struct serializer <std::chrono::system_clock::time_point, SERIALIZE_JSON>
		{
			static std::ostream& write(std::ostream& os, std::chrono::system_clock::time_point const& v);
		};
		
		template <>
		struct serializer <null, SERIALIZE_JSON>
		{
			static std::ostream& write(std::ostream& os, null v);
		};
		
		template <>
		struct serializer <std::int32_t, SERIALIZE_JSON>
		{
			static std::ostream& write(std::ostream& os, std::int32_t v);
		};
		
		template <>
		struct serializer <std::uint32_t, SERIALIZE_JSON>
		{
			static std::ostream& write(std::ostream& os, std::uint32_t v);
		};
		
		template <>
		struct serializer <std::int64_t, SERIALIZE_JSON>
		{
			static std::ostream& write(std::ostream& os, std::int64_t v);
		};
		
		template <>
		struct serializer <std::uint64_t, SERIALIZE_JSON>
		{
			static std::ostream& write(std::ostream& os, std::uint64_t v);
		};
		
		template <>
		struct serializer <float, SERIALIZE_JSON>
		{
			static std::ostream& write(std::ostream& os, float v);
		};
		
		template <>
		struct serializer <double, SERIALIZE_JSON>
		{
			static std::ostream& write(std::ostream& os, double v);
		};
	}
}

#endif // CYNG_IO_SERIALIZE_JSON_HPP


