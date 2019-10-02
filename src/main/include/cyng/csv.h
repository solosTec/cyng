/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 

#ifndef NODE_CSV_H
#define NODE_CSV_H


#include <cyng/core/object_interface_fwd.h>
#include <cyng/intrinsics/buffer.h>
#include <cyng/intrinsics/sets.h>
#include <string>

namespace cyng 
{
	namespace csv 
	{
		tuple_t read(std::string const&);
		tuple_t read(buffer_t const&);
		vector_t read_file(std::string const&);

		/**
		 * CSV has many restrictions to serialize data. 
		 * Most importantly, it can only map homogeneous lists.
		 * It really works well with a vector containing tuples
		 * of the same size.
		 */
		void write(std::ostream&, object const&);
		void write(std::ostream&, vector_t const&);
		bool write_file(std::string const&, vector_t const&);

		std::string to_string(object const&);
		std::string to_string(tuple_t const&);
		std::string to_string(vector_t const&);
	}
}
#endif
