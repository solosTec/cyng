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

		template <typename T, typename TAG /*= SERIALIZE_PLAIN*/>
		struct serializer 
		{		
			static std::ostream& write(std::ostream& os, T const& v)
			{
				os << v;
				return os;
			}
		};
	}	
}

#endif	//	CYNG_IO_H
