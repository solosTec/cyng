/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_PARSER_UTF8_HPP
#define CYNG_PARSER_UTF8_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <cyng/parser/utf8_parser.h>
#include <cyng/factory.h>

#include <boost/spirit/home/support/attributes.hpp>	//	transform_attribute
#include <boost/spirit/include/phoenix.hpp>	//	enable assignment of values like cyng::object

namespace boost		
{ 
	namespace spirit	
	{ 
		namespace traits	
		{ 
			//
			//	obsolete
			//	"auto-magically" conversion from UTF-32 to string
			//
			template <> // <typename Attrib, typename T, typename Enable>
			struct assign_to_attribute_from_value<std::string, cyng::utf::u32_string, void>
			{
				typedef u32_to_u8_iterator<cyng::utf::u32_string::const_iterator> Conv;

				static void call(cyng::utf::u32_string const& val, std::string& attr)
				{
					attr.assign(Conv(val.begin()), Conv(val.end()));
					std::cout << "assign_to_attribute_from_value<u32_string>: " << attr << std::endl;	
				}
			};
	
			//
			//	obsolete
			//
// 			template <> 
// 			struct transform_attribute< cyng::object, std::string, boost::spirit::qi::domain > 
// 			{ 
// 				typedef std::string type; 
// 				static type pre(cyng::object& v) 
// 				{ 
// 					return type(); 
// 				} 
// 				static void post(cyng::object& val, type const& attr) 
// 				{
// 					std::cout << "transform_attribute<string>: " << attr<< std::endl;
// 					val = cyng::make_object(attr);
// 				} 
// 				static void fail(cyng::object&) 
// 				{}
// 			}; 
 
			/**
			 * convert a utf-32 attribute into a object of type string
			 */
			template <>
			struct transform_attribute< cyng::object, cyng::utf::u32_string, boost::spirit::qi::domain >
			{
				typedef cyng::utf::u32_string type;
				static type pre(cyng::object& v)
				{
					return type();
				}
				static void post(cyng::object& val, type const& attr)
				{
// 					std::cout << "transform_attribute<u32_string>: " << cyng::utf::u32_to_u8_string(attr) << std::endl;
					val = cyng::make_object(cyng::utf::u32_to_u8_string(attr));
				}
				static void fail(cyng::object&)
				{}
			};

			/**
			 * Define a customization point with transform_attribute<> for
			 * boost::fusion::vector<cyng::utf::u32_string, cyng::utf::u32_string>
			 * aka a pair of strings.
			 */
			template <>
			struct transform_attribute< cyng::utf::u32_string_pair_t, boost::fusion::vector<cyng::utf::u32_string, cyng::utf::u32_string>, boost::spirit::qi::domain >
			{
				typedef boost::fusion::vector< cyng::utf::u32_string, cyng::utf::u32_string > type;
				static type pre(cyng::utf::u32_string_pair_t& pair)
				{
					return type();
				}
				static void post(cyng::utf::u32_string_pair_t& pair, type const& attr)
				{
					const cyng::utf::u32_string first = boost::fusion::front(attr);
					//for (auto c : first)
					//{
					//	std::wcout << (wchar_t)c;
					//	std::wcout.clear();
					//}
					//std::wcout << L":";
					const cyng::utf::u32_string second = boost::fusion::back(attr);
					//for (auto c : second)
					//{
					//	std::wcout << (wchar_t)c;
					//	std::wcout.clear();
					//}
					//std::wcout << L"\n";
					pair = std::make_pair(first, second);
				}
				static void fail(cyng::utf::u32_string_pair_t&)
				{}
			};
			
		}	//	traits
	}	//	spirit
}	//	boost
	
namespace cyng	
{
	namespace utf
	{		
		template <typename Iterator>
		char_parser< Iterator > ::char_parser()
			: char_parser::base_type(r_start)
		{
			// entry point
			r_start
				//	accept all characters but '"' and '\'
				= +(~boost::spirit::qi::standard_wide::char_(L"\"\\"))[boost::spirit::qi::_val += boost::spirit::qi::_1]
				// \ ( == reverse solidus)
				| boost::spirit::qi::lit(L"\x5C") >> r_char [boost::spirit::qi::_val += boost::spirit::qi::_1] 
				;
			
			r_char
				= boost::spirit::qi::lit(L"\x5C")[boost::spirit::qi::_val += L'\\']  	// \    reverse solidus U+005C
				| boost::spirit::qi::lit(L"\x2F")[boost::spirit::qi::_val += L'/'] 		// /    solidus         U+002F
				| boost::spirit::qi::lit(L"\x62")[boost::spirit::qi::_val += L'\b'] 	// b    backspace       U+0008
				| boost::spirit::qi::lit(L"\x66")[boost::spirit::qi::_val += L'\f'] 	// f    form feed       U+000C
				| boost::spirit::qi::lit(L"\x6E")[boost::spirit::qi::_val += L'\n'] 	// n    line feed       U+000A
				| boost::spirit::qi::lit(L"\x72")[boost::spirit::qi::_val += L'\r'] 	// r    carriage return U+000D
				| boost::spirit::qi::lit(L"\x74")[boost::spirit::qi::_val += L'\t'] 	// t    tab             U+0009
				// uXXXX - U+XXXX
				| boost::spirit::qi::lit(L"\x75") >> r_4hex_digit[boost::spirit::qi::_val += boost::spirit::qi::_1]
				;

		}

		template <typename Iterator>
		string_parser<Iterator>::string_parser()
			: string_parser::base_type(r_start)
		{
			r_start
				=  (*r_char);
		}
		
		template <typename Iterator, typename Skipper>
		quote_parser<Iterator, Skipper>::quote_parser()
			: quote_parser::base_type(r_start)
		{
			//  This statement requires to include the phoenix library.
			//	This works since r_string does not match the '"' symbol.
			//	Otherwise something like 
			//		quoted_string = '"' >> *(qi::char_ - '"') >> '"';
			//	is required.
			//
			r_start
				= boost::spirit::qi::lexeme[L'"' >> r_string >> L'"']
				;
		}

		template <typename Iterator, typename Skipper>
		pair_parser< Iterator, Skipper > ::pair_parser()
			: pair_parser::base_type(r_start)
		{
			// entry point
			r_start
				= r_pair //	transform_attribute<>(...)
				;

			r_pair
				= (*boost::spirit::standard_wide::alnum >> L':' >> r_value)
				;

			r_value
				= r_quote[boost::spirit::_val = boost::spirit::qi::_1]
				| boost::spirit::qi::lit(L"true")[boost::spirit::_val = boost::phoenix::construct<u32_string>(true_)]
				| boost::spirit::qi::lit(L"false")[boost::spirit::_val = boost::phoenix::construct<u32_string>(false_)]
				//	ToDo: add number
				;
		};
		
		template <typename Iterator, typename Skipper>
		obj_quote_parser<Iterator, Skipper>::obj_quote_parser()
			: obj_quote_parser::base_type(r_start)
		{
			r_start
				%= r_quote	//	transform_attribute<>(...)
				;
		}

	}
} 	//	cyng

#endif	// CYNG_PARSER_UTF8_HPP


