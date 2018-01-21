/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/intrinsics.h>
#include <cyng/io/parser/deserializer.h>
#include <cyng/factory.h>
#include <cyng/intrinsics/traits/tag.hpp>
#include <cyng/io/object_reader.hpp>
#include <boost/assert.hpp>

namespace cyng 
{
	namespace io
	{
		template <std::size_t N>
		object deserialize_object(std::istream& is)
		{
			using type = typename cyng::traits::reverse_type<N>::type;
			return make_object(read_object<type>(is));
		}
		
		object deserialize(std::istream& is, std::size_t tag, std::uint64_t size)
		{
			BOOST_ASSERT_MSG(size != 0, "invalid size");
			switch (tag)
			{
				case TC_NULL:
					BOOST_ASSERT(size == 0);
					return make_object();
				case TC_BOOL:			return deserialize_object<TC_BOOL>(is);
// 				case TC_CHAR:
				case TC_FLOAT:			return deserialize_object<TC_FLOAT>(is);
				case TC_DOUBLE:			return deserialize_object<TC_DOUBLE>(is);
				case TC_FLOAT80:		return deserialize_object<TC_FLOAT80>(is);
// 		// 		const char*,
				case TC_UINT8:			return deserialize_object<TC_UINT8>(is);
				case TC_UINT16:			
					BOOST_ASSERT(size == sizeof(std::uint16_t));
					return deserialize_object<TC_UINT16>(is);
				case TC_UINT32:			
					BOOST_ASSERT(size == sizeof(std::uint32_t));
					return deserialize_object<TC_UINT32>(is);
				case TC_UINT64:			return deserialize_object<TC_UINT64>(is);
				case TC_INT8:			return deserialize_object<TC_INT8>(is);
				case TC_INT16:			return deserialize_object<TC_INT16>(is);
				case TC_INT32:			return deserialize_object<TC_INT32>(is);
				case TC_INT64:			return deserialize_object<TC_INT64>(is);
				case TC_STRING:			return deserialize_object<TC_STRING>(is);
				case TC_TIME_POINT:		return deserialize_object<TC_TIME_POINT>(is);
				case TC_NANO_SECOND:	return deserialize_object<TC_NANO_SECOND>(is);
				case TC_MICRO_SECOND:	return deserialize_object<TC_MICRO_SECOND>(is);
				case TC_MILLI_SECOND:	return deserialize_object<TC_MILLI_SECOND>(is);
				case TC_SECOND:			return deserialize_object<TC_SECOND>(is);
				case TC_MINUTE:			return deserialize_object<TC_MINUTE>(is);
				case TC_HOUR:			return deserialize_object<TC_HOUR>(is);
// 				
 				case TC_DBL_TP:			return deserialize_object<TC_DBL_TP>(is);
 				case TC_VERSION:		return deserialize_object<TC_VERSION>(is);
 				case TC_REVISION:		return deserialize_object<TC_REVISION>(is);
 				case TC_CODE:			return deserialize_object<TC_CODE>(is);
 				case TC_BUFFER:			return deserialize_object<TC_BUFFER>(is);
 				case TC_MAC48:			return deserialize_object<TC_MAC48>(is);
 				case TC_MAC64:			return deserialize_object<TC_MAC64>(is);
// 				case TC_COLOR_8:			return deserialize_object<TC_COLOR_8>(is);
// 				case TC_COLOR_16:			return deserialize_object<TC_COLOR_16>(is);
 				
 				case TC_DIGEST_MD5:			return deserialize_object<TC_DIGEST_MD5>(is);
 				case TC_DIGEST_SHA1:		return deserialize_object<TC_DIGEST_SHA1>(is);
 				case TC_DIGEST_SHA256:		return deserialize_object<TC_DIGEST_SHA256>(is);
 				case TC_DIGEST_SHA512:		return deserialize_object<TC_DIGEST_SHA512>(is);
// 				

				//	The following data types will be serialized by there components
				//	and should therefore never show up here.
				case TC_OBJECT:
 				case TC_TUPLE:
				case TC_VECTOR:
				case TC_SET:
				case TC_ATTR_MAP:
				case TC_ATTR:
				case TC_PARAM_MAP:
				case TC_PARAM:
					BOOST_ASSERT_MSG(false, "wrong serialization sequence");
// 				
// 				case TC_EC		= type_tag_traits<boost::system::error_code>(),
				case TC_UUID:		return deserialize_object<TC_UUID>(is);
				case TC_FS_PATH:	return deserialize_object<TC_FS_PATH>(is);
				case TC_IP_TCP_ENDPOINT:	return deserialize_object<TC_IP_TCP_ENDPOINT>(is);
				case TC_IP_UDP_ENDPOINT:	return deserialize_object<TC_IP_UDP_ENDPOINT>(is);
				case TC_IP_ICMP_ENDPOINT:	return deserialize_object<TC_IP_ICMP_ENDPOINT>(is);
				case TC_IP_ADDRESS:		return deserialize_object<TC_IP_ADDRESS>(is);

				case TC_EOD:		
					BOOST_ASSERT(size == 0);
					return make_object(eod());
				
				default:
					break;
			}
			return make_object();
		}
	}
}



