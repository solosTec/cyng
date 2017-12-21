/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/docscript/parser.h>
#include <boost/spirit/home/support/attributes.hpp>	//	transform_attribute
#include <boost/phoenix.hpp>	//	enable assignment of values like cyy::object
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/include/all.hpp>
#include <iostream>

namespace boost
{
	namespace spirit
	{
		namespace traits
		{
			//
			//	"auto-magically" conversion from UTF-32 to string
			//
			template <> // <typename Attrib, typename T, typename Enable>
			struct assign_to_attribute_from_value<std::string, cyng::utf::u32_string, void>
			{
				typedef u32_to_u8_iterator<cyng::utf::u32_string::const_iterator> Conv;

				static void call(cyng::utf::u32_string const& val, std::string& attr)
				{
					attr.assign(Conv(val.begin()), Conv(val.end()));
				}
			};

			/**
			 * Define a customization point with transform_attribute<> for
			 * boost::fusion::vector<std::string, std::vector<std::string>>
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

		}
	}
}

namespace cyng	
{
	namespace docscript
	{
		template <typename Iterator>
		utf8_parser<Iterator>::utf8_parser()
			: utf8_parser::base_type(r_start)
		{
			//  this statement requires to include the phoenix library
			r_start
				%= *boost::spirit::qi::standard_wide::char_
				;
		}

		template <typename Iterator, typename Skipper>
		quoted_string_parser< Iterator, Skipper > ::quoted_string_parser()
			: quoted_string_parser::base_type(r_start)
		{
			// entry point
			r_start
				= r_string;

			r_string
				= boost::spirit::qi::lexeme[L'"' >> *r_char >> L'"'];

			r_char
				= +(~boost::spirit::qi::standard_wide::char_(L"\"\\"))[boost::spirit::qi::_val += boost::spirit::qi::_1]
				| boost::spirit::qi::lit(L"\x5C") >> (                    // \ (reverse solidus)
					boost::spirit::qi::lit(L"\x22")[boost::spirit::qi::_val += L'"'] | // "    quotation mark  U+0022
					boost::spirit::qi::lit(L"\x5C")[boost::spirit::qi::_val += L'\\'] | // \    reverse solidus U+005C
					boost::spirit::qi::lit(L"\x2F")[boost::spirit::qi::_val += L'/'] | // /    solidus         U+002F
					boost::spirit::qi::lit(L"\x62")[boost::spirit::qi::_val += L'\b'] | // b    backspace       U+0008
					boost::spirit::qi::lit(L"\x66")[boost::spirit::qi::_val += L'\f'] | // f    form feed       U+000C
					boost::spirit::qi::lit(L"\x6E")[boost::spirit::qi::_val += L'\n'] | // n    line feed       U+000A
					boost::spirit::qi::lit(L"\x72")[boost::spirit::qi::_val += L'\r'] | // r    carriage return U+000D
					boost::spirit::qi::lit(L"\x74")[boost::spirit::qi::_val += L'\t'] | // t    tab             U+0009
					boost::spirit::qi::lit(L"\x75")                         // uXXXX                U+XXXX
					>> _4HEXDIG[boost::spirit::qi::_val += boost::spirit::qi::_1]
					);

		}

		template <typename Iterator, typename Skipper>
		quote_parser< Iterator, Skipper > ::quote_parser()
			: quote_parser::base_type(r_start)
		{
			// entry point
			r_start
				= r_string
				;

			r_string
				= boost::spirit::qi::lexeme[L'"' >> *r_char >> L'"']
				;

			r_char
				= (boost::spirit::qi::standard_wide::char_ - L'"')[boost::spirit::qi::_val += boost::spirit::qi::_1]
				;

		};

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
				| boost::spirit::qi::lit(L"true")[boost::spirit::_val = boost::phoenix::construct<utf::u32_string>(true_)]
				| boost::spirit::qi::lit(L"false")[boost::spirit::_val = boost::phoenix::construct<utf::u32_string>(false_)]
				//	ToDo: add number
				;
		};


		template <typename Iterator>
		include_parser<Iterator>::include_parser()
			: include_parser::base_type(r_start)
		{
			r_start
				= boost::spirit::lit(".include")
				>> r_path[boost::spirit::_val = boost::phoenix::construct<boost::filesystem::path>(boost::spirit::_1)]
				;

			//	"\"<>:|?*" are invalid path symbols
			r_path
				%= '('
				>> +(boost::spirit::qi::ascii::char_ - boost::spirit::ascii::char_("\"<>():|?*"))
				>> ')'
				;

		}
	}
}	//	cyng


