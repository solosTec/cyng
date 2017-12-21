/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/json.h>
#include <cyng/object.h>
#include <cyng/json/json_parser.h>
#include <cyng/factory.h>
#include <cyng/io/serializer.h>
#include <cyng/factory.h>
#include <ios>
#include <iterator>

namespace cyng 
{
	namespace json 
	{
		object read(std::string const& inp)
		{
			std::pair<object, bool> r = parse_json(inp);
			return (r.second)
			? r.first
			: make_object()
			;
		}
		
		object read(buffer_t const& inp)
		{
			std::pair<object, bool> r = parse_json(inp);
			return (r.second)
			? r.first
			: make_object()
			;
		}
		
		object read_file(std::string const& p)
		{
			std::ifstream fs(p);
			if (fs.is_open())	
			{
				fs.unsetf(std::ios::skipws);
				
				//
				//	The double parenthesis are required to stop compiler see the string inp
				//	as a function.
				//
				const std::string inp((std::istream_iterator<char>(fs)), (std::istream_iterator<char>()));
				return read(inp);
			}
			return make_object();
		}
		
		void write(std::ostream& os, object const& obj)
		{
			io::serialize_json(os, obj);
		}
	}
}

