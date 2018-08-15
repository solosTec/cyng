/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Sylko Olzscher
 *
 */

#ifndef NODE_XML_READER_HPP
#define NODE_XML_READER_HPP


#include <pugixml.hpp>
#include <cyng/object.h>

namespace cyng
{
	namespace xml
	{
		cyng::object read_root(pugi::xml_node);
		cyng::object read(pugi::xml_node);
		cyng::object produce_object(std::string type, std::string value);
		bool is_set(std::string);
	}
}
#endif
