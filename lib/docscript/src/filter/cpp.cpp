/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Sylko Olzscher
 *
 */

#include <cyng/docscript/filter/cpp.h>
#include <cyng/docscript/parser.h>
#include <boost/regex/pending/unicode_iterator.hpp>
#include <cctype>
#include <iomanip>
#include <boost/uuid/uuid_io.hpp>

namespace cyng	
{
	namespace filter
	{
		cpp::cpp(int verbose, bool linenumbers, boost::uuids::uuid tag, std::size_t reserve)
			: linenumbers_(linenumbers)
			, verbose_(verbose)
			, tag_(tag)
			, result_()
			, state_(INITIAL_)
			, parenthesis_(0)
			, braces_(0)
			, pos_(0)
			, keywords_()
			, line_(0)
		{
			//
			//	initialize keyword list
			//
			keywords_.emplace(U"auto");
			keywords_.emplace(U"bool");
			keywords_.emplace(U"break");
			keywords_.emplace(U"case");
			keywords_.emplace(U"catch");
			keywords_.emplace(U"char");
			keywords_.emplace(U"class");
			keywords_.emplace(U"const");
			keywords_.emplace(U"const_cast");
			keywords_.emplace(U"continue");
			keywords_.emplace(U"decltype");
			keywords_.emplace(U"default");
			keywords_.emplace(U"delete");
			keywords_.emplace(U"do");
			keywords_.emplace(U"double");
			keywords_.emplace(U"dynamic_cast");
			keywords_.emplace(U"else");
			keywords_.emplace(U"enum");
			keywords_.emplace(U"explicit");
			keywords_.emplace(U"extern");
			keywords_.emplace(U"false");
			keywords_.emplace(U"float");
			keywords_.emplace(U"for");
			keywords_.emplace(U"friend");
			keywords_.emplace(U"goto");
			keywords_.emplace(U"if");
			keywords_.emplace(U"inline");
			keywords_.emplace(U"int");
			keywords_.emplace(U"long");
			keywords_.emplace(U"mutable");
			keywords_.emplace(U"namespace");
			keywords_.emplace(U"new");
			keywords_.emplace(U"nullptr");
			keywords_.emplace(U"operator");
			keywords_.emplace(U"private");
			keywords_.emplace(U"protected");
			keywords_.emplace(U"public");
			keywords_.emplace(U"register");
			keywords_.emplace(U"reinterpret_cast");
			keywords_.emplace(U"return");
			keywords_.emplace(U"short");
			keywords_.emplace(U"signed");
			keywords_.emplace(U"sizeof");
			keywords_.emplace(U"static");
			keywords_.emplace(U"static_assert");
			keywords_.emplace(U"switch");
			keywords_.emplace(U"template");
			keywords_.emplace(U"this");
			keywords_.emplace(U"throw");
			keywords_.emplace(U"true");
			keywords_.emplace(U"try");
			keywords_.emplace(U"typedef");
			keywords_.emplace(U"typeid");
			keywords_.emplace(U"typename");
			keywords_.emplace(U"union");
			keywords_.emplace(U"unsigned");
			keywords_.emplace(U"using");
			keywords_.emplace(U"virtual");
			keywords_.emplace(U"void");
			keywords_.emplace(U"volatile");
			keywords_.emplace(U"while");

			if (verbose_ > 3)
			{
				std::cout
					<< "***info: C++ keyword list initialized with "
					<< keywords_.size()
					<< " entries"
					<< std::endl;
			}

			//
			//	small optimization
			//
			result_.reserve(reserve * 2);
		}

		void cpp::put(std::uint32_t c)
		{
			switch (state_)
			{
			case QUOTE_:
				state_ = quote_state(c);
				break;
			case CHAR_:
				state_ = char_state(c);
				break;
			case SLASH_:
				state_ = slash_state(c);
				break;
			case COMMENT_:
				state_ = comment_state(c);
				break;
			case LITERAL_:
				state_ = literal_state(c);
				break;
			case NUMBER_:
				state_ = number_state(c);
				break;
			case PRE_:
				state_ = pre_state(c);
				break;
			case INCLUDE_:
				state_ = include_state(c);
				break;
			case INITIAL_:
			default:
				state_ = initial_state(c);
				break;
			}
		}

		std::string cpp::get_result()
		{
			if (verbose_ > 0)
			{
				if (parenthesis_ != 0)
				{
					std::cerr
						<< "***warning: C++ parentheses count not balanced "
						<< parenthesis_
						<< std::endl;
				}
				if (braces_ != 0)
				{
					std::cerr
						<< "***warning: C++ curly brackets count not balanced "
						<< braces_
						<< std::endl;
				}
			}

			return std::string(boost::u32_to_u8_iterator<u32_string::const_iterator>(result_.begin()), boost::u32_to_u8_iterator<u32_string::const_iterator>(result_.end())) + "</code>";
		}

		cpp::state cpp::initial_state(std::uint32_t c)
		{
			switch (c)
			{
			case '{':
				braces_++;
				//nl();
				//repeat(' ', braces_ * 2);
				result_.push_back('{');
				break;
			case '}':
				braces_--;
				//nl();
				//repeat(' ', braces_ * 2);
				result_.push_back('}');
				break;
			case '(':
				parenthesis_++;
				result_.push_back('(');
				break;
			case ')':
				parenthesis_--;
				result_.push_back(')');
				break;
			case '<':
				result_.append(U"&lt;");
				break;
			case '>':
				result_.append(U"&gt;");
				break;
			case '&':
				result_.append(U"&amp;");
				break;
			case '"':
				result_.append(U"&quot;");
				result_.insert(result_.end(), color_green_.begin(), color_green_.end());
				return QUOTE_;
			case '\'':
				result_.push_back(c);
				result_.insert(result_.end(), color_brown_.begin(), color_brown_.end());
				return CHAR_;
			case '/':
				return SLASH_;

			case '\t':
				repeat(' ', 2);
				break;

			case '#':
				pos_ = result_.size();
				result_.push_back(c);
				return PRE_;

			case '\n':
				if (line_ > 0) 	nl();
				else ++line_;
				break;

			default:
				if ((c < 255) && std::isalpha(c))
				{
					pos_ = result_.size();
					result_.push_back(c);
					return LITERAL_;
				}
				else if ((c < 255) && std::isdigit(c))
				{
					pos_ = result_.size();
					result_.push_back(c);
					return NUMBER_;
				}
				else
				{
					result_.push_back(c);
				}
				break;
			}
			return INITIAL_;
		}

		cpp::state cpp::literal_state(std::uint32_t c)
		{
			switch (c)
			{
			case ')':
				parenthesis_--;
				stop_keyword();
				result_.push_back(c);
				return INITIAL_;
			case '(':
				parenthesis_++;
				stop_keyword();
				result_.push_back(c);
				return INITIAL_;
			case '{':
				braces_++;
				stop_keyword();
				result_.push_back(c);
				return INITIAL_;
			case '}':
				braces_--;
				stop_keyword();
				result_.push_back(c);
				return INITIAL_;
			case ' ':
			case ':':
			case ';':
			case '+':
			case '-':
			case '*':
			case '/':
				stop_keyword();
				result_.push_back(c);
				return INITIAL_;

			case '&':
				stop_keyword();
				//	&amp; 
				result_.append(U"&amp;");
				return INITIAL_;

			case '\n':
				stop_keyword();
				nl();
				return INITIAL_;

			case '<':
				if (is_keyword())
				{
					result_.insert(pos_, color_blue_);
					result_.insert(result_.end(), end_.begin(), end_.end());
				}
				result_.append(U"&lt;");
				//result_.push_back(' ');
				return INITIAL_;
			case '>':
				if (is_keyword())
				{
					result_.insert(pos_, color_blue_);
					result_.insert(result_.end(), end_.begin(), end_.end());
				}
				//result_.push_back(' ');
				result_.append(U"&gt;");
				return INITIAL_;
			case '\t':
				if (stop_keyword()) {
					repeat(' ', 1);
				}
				else {
					repeat(' ', 2);
				}
				return INITIAL_;
			default:
				result_.push_back(c);
				break;
			}
			return LITERAL_;
		}

		bool cpp::stop_keyword()
		{
			if (is_keyword())
			{
				result_.insert(pos_, color_blue_);
				result_.insert(result_.end(), end_.begin(), end_.end());
				return true;
			}
			return false;
		}

		cpp::state cpp::number_state(std::uint32_t c)
		{
			if (std::isdigit(c) || (c == '.'))
			{
				result_.push_back(c);
				return NUMBER_;
			}

			switch (c)
			{
			case '{':
				braces_++;
				break;
			case '}':
				braces_--;
				break;
			case '(':
				parenthesis_++;
				break;
			case ')':
				parenthesis_--;
				break;
			}

			//
			//	brown numbers
			//
			result_.insert(pos_, color_brown_);
			result_.insert(result_.end(), end_.begin(), end_.end());

			result_.push_back(c);
			return INITIAL_;
		}

		cpp::state cpp::quote_state(std::uint32_t c)
		{
			switch (c)
			{
			case '"':
				result_.insert(result_.end(), end_.begin(), end_.end());
				result_.append(U"&quot;");
				return INITIAL_;

			default:
				result_.push_back(c);
				break;
			}
			return QUOTE_;
		}

		cpp::state cpp::char_state(std::uint32_t c)
		{
			switch (c)
			{
			case '\'':
				result_.insert(result_.end(), end_.begin(), end_.end());
				result_.push_back(c);
				return INITIAL_;

			default:
				result_.push_back(c);
				break;
			}
			return CHAR_;
		}

		cpp::state cpp::slash_state(std::uint32_t c)
		{
			if (c != '/')
			{
				result_.push_back('/');
				result_.push_back(c);
				return INITIAL_;
			}
			pos_ = result_.size();
			result_.push_back(c);
			result_.push_back(c);
			return COMMENT_;
		}

		cpp::state cpp::comment_state(std::uint32_t c)
		{
			if (c == '\n')
			{
				result_.insert(pos_, color_grey_);
				result_.insert(result_.end(), end_.begin(), end_.end());
				nl();
				return INITIAL_;
			}
			result_.push_back(c);
			return COMMENT_;
		}

		cpp::state cpp::pre_state(std::uint32_t c)
		{
			if ((c < 255) && std::isalpha(c))
			{
				result_.push_back(c);
				return PRE_;
			}
			else if (c == ' ' || c == '\t')
			{
				//	skip spaces
				return PRE_;
			}
			else if (c == '<')
			{
				result_.insert(pos_, color_grey_);
				result_.insert(result_.end(), end_.begin(), end_.end());

				result_.push_back(' ');
				result_.append(U"&lt;");
				pos_ = result_.size();
				return INCLUDE_;
			}
			else if (c == '"')
			{
				result_.insert(pos_, color_grey_);
				result_.insert(result_.end(), end_.begin(), end_.end());

				result_.push_back(' ');
				result_.append(U"&quot;");
				pos_ = result_.size();
				return INCLUDE_;
			}

			//
			//	other
			//
			result_.insert(pos_, color_red_);
			result_.insert(result_.end(), end_.begin(), end_.end());
			result_.push_back(' ');
			result_.push_back(c);
			return INITIAL_;
		}

		cpp::state cpp::include_state(std::uint32_t c)
		{
			if (c == '>')
			{
				result_.insert(pos_, color_red_);
				result_.insert(result_.end(), end_.begin(), end_.end());
				result_.append(U"&gt;");
				return INITIAL_;
			}
			else if (c == '"')
			{
				result_.insert(pos_, color_brown_);
				result_.insert(result_.end(), end_.begin(), end_.end());
				result_.push_back(c);
				return INITIAL_;
			}
			else if (c == '\n')
			{
				nl();
				return INITIAL_;
			}
			result_.push_back(c);
			return INCLUDE_;
		}

		void cpp::repeat(std::uint32_t c, std::size_t count)
		{
			while (count-- != 0)
			{
				result_.push_back(c);
			}
		}

		bool cpp::is_keyword() const
		{
			if (pos_ < result_.size())
			{
				if (verbose_ > 4)
				{
					std::cout
						<< "***info: C++ lookup "
						<< std::string(&result_[pos_], &result_[result_.size()])
						<< std::endl;
				}

				return keywords_.find(result_.substr(pos_)) != keywords_.end();
			}
			return false;
		}

		void cpp::nl()
		{
			if (!result_.empty()) {
				result_.append(U"</code>");
			}

			result_.push_back('\n');
			result_.append(U"<code>");

			//
			//	write line number
			//
			if (linenumbers_)
			{
				std::stringstream ss;
				ss
					<< "<span style = \"color: DarkCyan; font-size: smaller;\" id=\""
					<< tag_
					<< '-'
					<< line_
					<< "\">"
					<< std::setw(4)
					<< std::setfill('0')
					<< line_
					<< "</span>"
					;
				auto nr = docscript::parse_utf8(ss.str());
				result_.insert(result_.end(), nr.begin(), nr.end());
				result_.push_back(' ');
			}

			line_++;
		}

		//
		//	constants
		//
		u32_string const cpp::color_green_ = U"<span style=\"color: green;\">";
		u32_string const cpp::color_blue_ = U"<span style=\"color: blue;\">";
		u32_string const cpp::color_grey_ = U"<span style=\"color: grey;\">";
		u32_string const cpp::color_red_ = U"<span style=\"color: red;\">";
		u32_string const cpp::color_cyan_ = U"<span style=\"color: DarkCyan; font-size: smaller;\">";
		u32_string const cpp::color_brown_ = U"<span style=\"color: brown;\">";
		u32_string const cpp::end_ = U"</span>";
	}
}	//	cyng


