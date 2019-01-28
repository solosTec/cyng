/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Sylko Olzscher
 *
 */

#include <cyng/docscript/filter/json.h>
#include <cyng/json.h>
#include <cyng/dom/tree_walker.h>
#include <cyng/object.h>
#include <cyng/core/class_interface.h>
#include <cyng/intrinsics/traits/tag.hpp>

#include <iostream>
#include <iomanip>

#include <boost/regex/pending/unicode_iterator.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace cyng	
{
	namespace filter
	{
		json::json(bool linenumbers, boost::uuids::uuid tag, std::size_t reserve)
			: linenumbers_(linenumbers)
			, tag_(tag)
			, result_()
		{
			result_.reserve(reserve);
		}

		std::string json::get_result()
		{
			return result_;
		}

		void json::put(std::string const& inp)
		{
			//
			//	parse into an object hierarchy
			//
			auto obj = cyng::json::read(inp);

			//
			//	walk over object tree and generate formatted JSON output
			//
			std::stringstream ss;
			json_walker walker(ss, linenumbers_, tag_);
			traverse(obj, walker, 0);
			ss << "</code>";
			result_ = ss.str();
		}

		json_walker::json_walker(std::ostream& os, bool linenumbers, boost::uuids::uuid tag)
			: os_(os)
			, linenumbers_(linenumbers)
			, tag_(tag)
			, line_(0)
		{
			if (linenumbers_) {
				os_ << nl();
			}
		}

		bool json_walker::enter_node(std::size_t depth, object const& obj, std::size_t idx, std::size_t total, object parent)
		{

			//std::cout << "enter " << depth << " - " << idx << '/' << total << " - " << obj.get_class().type_name() << " - " << cyng::json::to_string(obj) << std::endl;

			switch (obj.get_class().tag()) {
			case TC_TUPLE:
				//
				//	If the parent element was a also a container insert a NL.
				//	
				if (parent.get_class().tag() == TC_TUPLE || parent.get_class().tag() == TC_VECTOR) {
					os_
						<< nl()
						<< indentation(depth)
						;
				}
				os_ << '{';
				break;
			case TC_VECTOR:
				os_ << '[';
				break;
			case TC_PARAM:
			{
				using type = typename std::tuple_element<type_code::TC_PARAM, cyng::traits::tag_t>::type;
				const type* ptr = object_cast<type>(obj);
				BOOST_ASSERT_MSG(ptr != nullptr, "invalid type info");
				os_
					<< nl()
					<< indentation(depth)
					<< "<span style = \"color: blue\">"
					<< "&quot;"
					<< ptr->first
					<< "&quot;"
					<< "</span>"
					<< ':'
					<< ' '
					;
			}
			break;

			case TC_STRING:
				os_
					<< "<span style = \"color: brown\">"
					<< cyng::json::to_string(obj)
					<< "</span>"
					;
				break;
			default:
				os_ << cyng::json::to_string(obj);
				break;
			}
			return true;	//	continue
		}

		void json_walker::leave_node(std::size_t depth, object const& obj, std::size_t idx, std::size_t total, object previous)
		{
			//std::cout << "leave " << depth << " - " << idx << '/' << total << " - "  << obj.get_class().type_name() << " - " << cyng::json::to_string(obj) << std::endl;

			switch (obj.get_class().tag()) {
			case TC_TUPLE:
				os_ 
					<< nl()
					<< indentation(depth)
					<< '}'
					;
				if ((idx != 1) && (depth != 0))	os_ << ',';
				//if ((idx != 1) && total != 1) {
				//	os_
				//		<< std::endl
				//		<< indentation(depth)
				//		;
				//}
				break;
			case TC_VECTOR:
				//
				//	If the previous element was a also a container insert a NL.
				//	
				if (previous.get_class().tag() == TC_TUPLE || previous.get_class().tag() == TC_VECTOR) {
					os_
						<< nl()
						<< indentation(depth)
						;
				}
				os_	<< ']';
				if (idx != 1)	os_ << ',';
				break;
			case TC_PARAM:
			case TC_ATTR:
				break;
			default:
				if (idx != 1)	os_ << ',';
				break;
			}
		}

		std::string json_walker::indentation(std::size_t depth)
		{
			return (depth == 0)
				? ""
				: "  " + indentation(depth - 1)
				;
		}

		std::string json_walker::nl()
		{
			std::stringstream ss;
			if (line_ != 0u) {
				ss << "</code>";
			}
			ss 
				<< std::endl
				<< "<code>"
				;
			++line_;
			if (linenumbers_)
			{
				ss
					<< "<span style = \"color: DarkCyan; font-size: smaller;\" id=\""
					<< tag_
					<< '-'
					<< line_
					<< "\">"
					<< std::setw(4)
					<< std::setfill('0')
					<< line_
					<< "</span> "
					;
			}
			return ss.str();
		}


	}
}	//	cyng


