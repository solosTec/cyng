/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#ifndef NODE_XML_H
#define NODE_XML_H


#include <cyng/intrinsics/buffer.h>
#include <cyng/intrinsics/sets.h>
#include <string>
#include <pugixml.hpp>

namespace cyng 
{
	namespace xml 
	{
		/** 
		 * For correct representation of NULL values the XML namespace
		 * xmlns:xsi = "w3.org/2001/XMLSchema-instance" 
		 * must be declared.
		 *
		 * @see https://www.w3.org/TR/2004/REC-xmlschema-2-20041028/datatypes.html#built-in-datatypes
		 */
		void write(pugi::xml_node, object const&);
		void write(pugi::xml_node, tuple_t const&);

		object read_file(std::string const&);
		//object read(std::string const&);
		//object read(buffer_t const&);
	}
}
#endif
