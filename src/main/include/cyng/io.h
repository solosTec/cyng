/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_IO_H
#define CYNG_IO_H


#include <ostream>
#include <type_traits>

namespace cyng 
{
	class object;
	namespace io
	{
		//
		//	serialization tags
		//
		
		/**
		 * Simple serialization to human readable text 
		 */
		struct SERIALIZE_PLAIN {};
		
		/**
		 * Serialization to readable output
		 * with type information 
		 */
		struct SERIALIZE_TYPED {};
		
		/**
		 * Serialization to an internal binary 
		 * format with TLV (type-length-value) encoding.
		 */
		struct SERIALIZE_BINARY {};
	
		/**
		 * Serialization to JSON.
		 */
		struct SERIALIZE_JSON {};
		//struct SERIALIZE_JSON_PRETTY {};

		/**
		 * Serialization to XML.
		 */
		struct SERIALIZE_XML {};

		/**
		 * Serialization to CSV.
		 */
		struct SERIALIZE_CSV {};

		template <typename T, typename TAG>
		struct serializer 
		{		
			static std::ostream& write(std::ostream& os, T const& v)
			{
				os << v;
				return os;
			}
		};

		template <typename TAG>
		struct serializer_custom
		{
			static std::ostream& write(std::ostream& os, std::size_t tag, std::string const& type_name, object const& obj)
			{
				os
					<< "<!"
					<< tag
					<< ':'
					<< type_name
					<< '>'
					;
				return os;
			}
		};

	}	
}

#endif	//	CYNG_IO_H
