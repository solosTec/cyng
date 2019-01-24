/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_IO_SERIALIZE_PLAIN_HPP
#define CYNG_IO_SERIALIZE_PLAIN_HPP


#include <cyng/io.h>
#include <cyng/intrinsics.h>
#include <iostream>
#include <iomanip>


namespace cyng 
{	
	namespace io
	{
		
		template <>
		struct serializer <bool, SERIALIZE_PLAIN>
		{
			static std::ostream& write(std::ostream& os, bool v);
		};
		
		template <>
		struct serializer <buffer_t, SERIALIZE_PLAIN>
		{
			static std::ostream& write(std::ostream& os, buffer_t const& v);
		};
		
		template <>
		struct serializer <boost::uuids::uuid, SERIALIZE_PLAIN>
		{
			static std::ostream& write(std::ostream& os, boost::uuids::uuid const& v);
		};

		template <>
		struct serializer <tuple_t, SERIALIZE_PLAIN>
		{
			static std::ostream& write(std::ostream& os, tuple_t const& v);
		};

		template <>
		struct serializer <vector_t, SERIALIZE_PLAIN>
		{
			static std::ostream& write(std::ostream& os, vector_t const& v);
		};

		template <>
		struct serializer <set_t, SERIALIZE_PLAIN>
		{
			static std::ostream& write(std::ostream& os, set_t const& v);
		};


 		template <>
 		struct serializer <attr_map_t, SERIALIZE_PLAIN>
 		{
 			static std::ostream& write(std::ostream& os, attr_map_t const& v);
 		};
 		
 		template <>
 		struct serializer <param_map_t, SERIALIZE_PLAIN>
 		{
 			static std::ostream& write(std::ostream& os, param_map_t const& v);
 		};

		template <>
		struct serializer <color_8, SERIALIZE_PLAIN>
		{
			static std::ostream& write(std::ostream& os, color_8 const& v);
		};
		
		template <>
		struct serializer <color_16, SERIALIZE_PLAIN>
		{
			static std::ostream& write(std::ostream& os, color_16 const& v);
		};
		
 		template <>
 		struct serializer <std::string, SERIALIZE_PLAIN>
 		{
 			static std::ostream& write(std::ostream& os, std::string const& v);
 		};

		template <>
		struct serializer <std::int8_t, SERIALIZE_PLAIN>
		{
			static std::ostream& write(std::ostream& os, std::int8_t v);
		};

		template <>
		struct serializer <std::uint8_t, SERIALIZE_PLAIN>
		{
			static std::ostream& write(std::ostream& os, std::uint8_t v);
		};

		template <>
		struct serializer <std::uint16_t, SERIALIZE_PLAIN>
		{
			static std::ostream& write(std::ostream& os, std::uint16_t v);
		};

		template <>
		struct serializer <std::uint32_t, SERIALIZE_PLAIN>
		{
			static std::ostream& write(std::ostream& os, std::uint32_t v);
		};

		/**
		 * serialize custom types
		 */
		template <>
		struct serializer_custom<SERIALIZE_PLAIN>
		{
			static std::ostream& write(std::ostream& os, std::size_t tag, std::string const& type_name, object const& obj);
		};
	}
}

#endif // CYNG_IO_SERIALIZE_PLAIN_HPP
