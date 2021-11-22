/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher
 *
 */ 

#ifndef CYNG_XML_H
#define CYNG_XML_H

#include <cyng/obj/object.h>
#include <cyng/store/record.h>
#include <cyng/store/meta.h>

#include <pugixml.hpp>

namespace cyng 
{
	namespace xml 
	{
		/**
		 * read an XML file into an object
		 */
		object read_file(std::string const&);

		/**
		 * Generic way to read a record.
		 * Example:
		 * @code
		 pugi::xpath_node_set data = doc.select_nodes("root/record");
		 for (pugi::xpath_node_set::const_iterator it = data.begin(); it != data.end(); ++it)
		 {
			counter++;
			pugi::xml_node node = it->node();
			auto rec = read(node, meta);
		 }
		 * @endcode
		 */
		cyng::record read(pugi::xml_node, meta_store const&);
	}
}
#endif
