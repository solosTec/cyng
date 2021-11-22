/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#ifndef CYNG_XML_READER_H
#define CYNG_XML_READER_H


#include <pugixml.hpp>
#include <cyng/obj/object.h>

namespace cyng
{
	namespace xml
	{
		cyng::object read_root(pugi::xml_node);
		cyng::object read(pugi::xml_node);
		cyng::object produce_object(std::string type, std::string value);
		bool is_set(std::string);
		cyng::object read(pugi::xml_node);
	}
}
#endif
