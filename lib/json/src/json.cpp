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
#include <cyng/io/serializer/json.hpp>
#include <cyng/factory.h>
#include <cyng/dom/tree_walker.h>
#include <ios>
#include <iterator>

namespace cyng 
{
	namespace json 
	{
		object read(std::string const& inp)
		{
			auto const r = parse_json(inp);
			return (r.second)
			? r.first
			: make_object()
			;
		}
		
		object read(buffer_t const& inp)
		{
			auto const r = parse_json(inp);
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
				//std::string const inp((std::istream_iterator<char>(fs)), (std::istream_iterator<char>()));
				std::string const inp{ std::istream_iterator<char>(fs), std::istream_iterator<char>() };
				return read(inp);
			}
			return make_object();
		}
		
		void write(std::ostream& os, object const& obj)
		{
			io::serialize_json(os, obj);
		}

		void write(std::ostream& os, tuple_t const& tpl)
		{
			cyng::io::serializer <tuple_t, cyng::io::SERIALIZE_JSON>::write(os, tpl);
		}

		std::string to_string(object const& obj)
		{
			std::stringstream ss;
			write(ss, obj);
			return ss.str();
		}

		std::string to_string(tuple_t const& tpl)
		{
			std::stringstream ss;
			cyng::io::serializer <tuple_t, cyng::io::SERIALIZE_JSON>::write(ss, tpl);
			return ss.str();
		}

		std::string to_string(vector_t const& tpl)
		{
			std::stringstream ss;
			cyng::io::serializer <vector_t, cyng::io::SERIALIZE_JSON>::write(ss, tpl);
			return ss.str();
		}

		class json_walker : public tree_walker
		{
		public:
			json_walker(std::ostream& os)
				: os_(os)
			{}

			virtual bool enter_node(std::size_t depth
				, cyng::object const& obj
				, std::size_t total
				, bool last
				, cyng::object parent) override
			{

				switch (obj.get_class().tag()) {
				case cyng::TC_TUPLE:
					//
					//	If the parent element was a also a container insert a NL.
					//	
					if (parent.get_class().tag() == cyng::TC_TUPLE || parent.get_class().tag() == cyng::TC_VECTOR) {
						os_
							<< std::endl
							<< indentation(depth)
							;
					}
					os_ << '{';
					break;
				case cyng::TC_VECTOR:
					os_ << '[';
					break;
				case cyng::TC_PARAM:
				{
					using type = typename std::tuple_element<cyng::type_code::TC_PARAM, cyng::traits::tag_t>::type;
					const type* ptr = cyng::object_cast<type>(obj);
					BOOST_ASSERT_MSG(ptr != nullptr, "invalid type info");
					os_
						<< std::endl
						<< indentation(depth)
						<< ptr->first
						<< ':'
						<< ' '
						;
				}
				break;

				case cyng::TC_STRING:
					os_
						<< cyng::json::to_string(obj)
						;
					break;
				default:
					os_ << cyng::json::to_string(obj);
					break;
				}
				return true;	//	continue
			}

			virtual void leave_node(std::size_t depth
				, cyng::object const& obj
				, std::size_t total
				, bool last
				, std::size_t size) override
			{
				switch (obj.get_class().tag()) {
				case cyng::TC_TUPLE:
					if (size > 0) {
						os_
							<< std::endl
							<< indentation(depth)
							;
					}
					os_ << '}' ;
					if (!last && (depth != 0))	os_ << ',';
					break;
				case cyng::TC_VECTOR:
					//
					//	If the previous element was a also a container insert a NL.
					//	
					if (size > 0) {
						os_
							<< std::endl
							<< indentation(depth)
							;
					}
					os_ << ']';
					if (!last)	os_ << ',';
					break;
				case cyng::TC_PARAM:
				case cyng::TC_ATTR:
					break;
				default:
					if (!last)	os_ << ',';
					break;
				}
			}

		private:
			std::string indentation(std::size_t depth)
			{
				return (depth == 0)
					? ""
					: "  " + indentation(depth - 1)
					;
			}

		private:
			std::ostream& os_;
		};

		void pretty_print(std::ostream& os, object const& obj)
		{
			json_walker walker(os);
			traverse(obj, walker, 0);
		}

	}
}

