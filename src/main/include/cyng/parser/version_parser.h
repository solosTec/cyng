/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_PARSER_VERSION_H
#define CYNG_PARSER_VERSION_H

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <cyng/object.h>
#include <cyng/intrinsics/version.h>

namespace cyng
{
	
	std::pair<version, bool> parse_version(std::string);
	std::pair<revision, bool> parse_revision(std::string);
	std::pair<object, bool> parse_ver(std::string inp);

	/**
	 * Accept input of the the form uint16.uint16
	 * and returns a value of type version.
	 * 
	 * @see cyng::version
	 */
	template <typename Iterator>
	struct ver_parser
	: boost::spirit::qi::grammar<Iterator, version()>
	{
		ver_parser();
		boost::spirit::qi::rule<Iterator, version()> r_start;

		/**
		 * read parts of version
		 */
		boost::spirit::qi::uint_parser<std::uint16_t, 10, 1, 5> 	r_uint16;
			
	};
		
	template <typename Iterator>
	struct rev_parser
	: boost::spirit::qi::grammar<Iterator, revision()>
	{
		rev_parser();
		boost::spirit::qi::rule<Iterator, revision()> r_start;

		/**
		 * read parts of revision
		 */
		boost::spirit::qi::uint_parser<std::uint16_t, 10, 1, 5> 	r_uint16;
			
	};

	template <typename Iterator>
	struct version_parser
	: boost::spirit::qi::grammar<Iterator, object()>
	{
		version_parser();
		boost::spirit::qi::rule<Iterator, object()> r_start;

		boost::spirit::qi::rule<Iterator, object()> r_version;
		boost::spirit::qi::rule<Iterator, object()> r_revision;
			
		/**
		 * read parts of version/revision
		 */
		boost::spirit::qi::uint_parser<std::uint16_t, 10, 1, 5> 	r_uint16;
			
	};
}

#endif

