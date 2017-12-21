/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef NODE_JSON_H
#define NODE_JSON_H


#include <cyng/core/object_interface_fwd.h>
#include <cyng/intrinsics/buffer.h>
#include <string>

namespace cyng 
{
	namespace json 
	{
		object read(std::string const&);
		object read(buffer_t const&);
		object read_file(std::string const&);
		void write(std::ostream&, object const&);
	}	
}
#endif
