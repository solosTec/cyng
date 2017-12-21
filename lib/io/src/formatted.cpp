/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/io/serializer/formatted.hpp>
#include <cyng/io/serializer.h>
#include <cyng/object.h>
#include <boost/io/ios_state.hpp>

namespace cyng 
{	
	namespace io
	{
		
		std::ostream& serializer <version, SERIALIZE_FORMATTED>::write(std::ostream& os, version const& v)
		{
			os << cyng::traits::get_tag_name<version>();
			serializer<version, SERIALIZE_PLAIN>::write(os, v);
			return os;		
		}
		
		std::ostream& serializer <revision, SERIALIZE_FORMATTED>::write(std::ostream& os, revision const& v)
		{
			os << cyng::traits::get_tag_name<revision>();
			serializer<revision, SERIALIZE_PLAIN>::write(os, v);
			return os;		
		}
	}
}


