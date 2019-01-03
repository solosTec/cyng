/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Sylko Olzscher
 *
 */

#include <cyng/docscript/filter/verbatim.h>
#include <boost/regex/pending/unicode_iterator.hpp>

namespace cyng	
{
	namespace filter
	{
		verbatim::verbatim(std::size_t reserve)
			: result_()
		{
			result_.reserve(reserve);
		}

		std::string verbatim::get_result()
		{
			return std::string(boost::u32_to_u8_iterator<u32_string::const_iterator>(result_.begin()), boost::u32_to_u8_iterator<u32_string::const_iterator>(result_.end()));
		}

		void verbatim::put(std::uint32_t c)
		{
			switch (c)
			{
			case '<':
				result_.push_back('&');
				result_.push_back('#');
				result_.push_back('6');
				result_.push_back('0');
				result_.push_back(';');
				break;
			case '>':
				result_.push_back('&');
				result_.push_back('#');
				result_.push_back('6');
				result_.push_back('2');
				result_.push_back(';');
				break;
			case '&':
				result_.push_back('&');
				result_.push_back('#');
				result_.push_back('3');
				result_.push_back('8');
				result_.push_back(';');
				break;
			case '"':
				result_.push_back('&');
				result_.push_back('#');
				result_.push_back('3');
				result_.push_back('4');
				result_.push_back(';');
				break;
			default:
				result_.push_back(c);
				break;
			}
		}
	}
}	//	cyng


