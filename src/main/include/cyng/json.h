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
#include <cyng/intrinsics/sets.h>
#include <string>

namespace cyng 
{
	namespace json 
	{
		object read(std::string const&);
		object read(buffer_t const&);
		object read_file(std::string const&);

		void write(std::ostream&, object const&);
		void write(std::ostream&, tuple_t const&);

		/**
		 * Serialize an object to an JSON string. 
		 * @note not all data structures can be properly transformed
		 * to JSON. Each data element needs a key/value envelop. Exceptions
		 * are null and boolean values or elements are wrapped in an array.
		 */
		std::string to_string(object const&);
		std::string to_string(tuple_t const&);
		std::string to_string(vector_t const&);
		std::string to_string(param_map_t const&);

		/**
		 * Works for other content than JSON (partially) too.
		 */
		void pretty_print(std::ostream&, object const&);
	}
}
#endif
