/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#include <cyng/io/serializer/cpp.hpp>
#include <cyng/io/serialize.h>
#include <cyng/obj/tag.hpp>
#include <cyng/obj/object.h>
#include <cyng/io/ostream.h>
#include <cyng/io/parser/utf-8.h>

#include  <iomanip>

#include <boost/io/ios_state.hpp>

namespace cyng {
	namespace io {

		std::size_t serializer <null, CPP>::write(std::ostream& os, null)
		{
			calc_size const cs(os);
			os << "cyng::make_object()";
			return cs;
		}

		std::size_t serializer <eod, CPP>::write(std::ostream& os, eod)
		{
			calc_size const cs(os);
			os << "cyng::make_eod())";	//	factory.hpp
			return cs;
		}

		std::size_t serializer <bool, CPP>::write(std::ostream& os, bool v)
		{
			//	true/false
			calc_size const cs(os);
			os << (v ? "true" : "false");
			return cs;
		}

		//std::size_t serializer <char, CPP>::write(std::ostream& os, char v)
		//{
		//	calc_size const cs(os);
		//	os << '\'' << v << '\'';
		//	return cs;
		//}

		std::size_t serializer <std::int8_t, CPP>::write(std::ostream& os, std::int8_t v)
		{
			calc_size const cs(os);
			os << "static_cast<std::int8_t>(0x" 
				<< std::dec
				<< +v
				<< ")"
				;
			return cs;
		}

		std::size_t serializer <std::int16_t, CPP>::write(std::ostream& os, std::int16_t v)
		{
			calc_size const cs(os);
			os << "static_cast<std::int16_t>(0x"
				<< std::dec
				<< v
				<< ")"
				;
			return cs;
		}

		std::size_t serializer <std::int32_t, CPP>::write(std::ostream& os, std::int32_t v)
		{
			calc_size const cs(os);
			os << "static_cast<std::int32_t>( 0x"
				<< std::dec
				<< v
				<< " )"
				;
			return cs;
		}

		std::size_t serializer <std::int64_t, CPP>::write(std::ostream& os, std::int64_t v)
		{
			calc_size const cs(os);
			os << "static_cast<std::int64_t>( 0x"
				<< std::dec
				<< v
				<< " )"
				;
			return cs;
		}

		std::size_t serializer <std::uint8_t, CPP>::write(std::ostream& os, std::uint8_t v)
		{
			calc_size const cs(os);
			boost::io::ios_flags_saver  ifs(os);

			os
				<< "static_cast<std::int8_t>( 0x"
				<< std::hex
				<< +v
				<< " )";
				;

			return cs;
		}
		std::size_t serializer <std::uint16_t, CPP>::write(std::ostream& os, std::uint16_t v)
		{
			calc_size const cs(os);
			boost::io::ios_flags_saver  ifs(os);

			os
				<< "static_cast<std::uint16_t>( 0x"
				<< std::hex
				<< v
				<< " )"
				;

			return cs;
		}
		std::size_t serializer <std::uint32_t, CPP>::write(std::ostream& os, std::uint32_t v)
		{
			calc_size const cs(os);
			boost::io::ios_flags_saver  ifs(os);

			os
				<< "static_cast<std::uint32_t>( 0x"
				<< std::hex
				<< v
				<< " )"
				;

			return cs;
		}

		std::size_t serializer <std::uint64_t, CPP>::write(std::ostream& os, std::uint64_t v)
		{
			calc_size const cs(os);
			boost::io::ios_flags_saver  ifs(os);

			os
				<< "static_cast<std::uint64_t>( 0x"
				<< std::hex
				<< v
				<< " )"
				;

			return cs;
		}

		std::size_t serializer <double, CPP>::write(std::ostream& os, double v)
		{
			calc_size const cs(os);
			boost::io::ios_flags_saver  ifs(os);

			os
				<< "static_cast<std::uint64_t>( 0x"
				<< std::fixed
				<< v
				<< " )"
				;

			return cs;
		}

		std::size_t serializer <std::string, CPP>::write(std::ostream& os, std::string const& str)
		{
			calc_size const cs(os);
			//	store and reset stream state
			boost::io::ios_flags_saver  ifs(os);

			os
				<< std::setfill('0')
				<< std::hex
				<< '"'
				;

			utf8::u8_to_u32_iterator begin(str.begin());
			utf8::u8_to_u32_iterator end(str.end());
			for (; begin != end; ++begin) {
				auto const c = *begin;

				switch (c)
				{
				case '\a':
					os << '\\' << 'a';
					break;
				case '\b':
					os << '\\' << 'b';
					break;
				case '\f':
					os << '\\' << 'f';
					break;
				case '\n':
					os << '\\' << 'n';
					break;
				case '\r':
					os << '\\' << 'r';
					break;
				case '\t':
					os << '\\' << 't';
					break;
				case '\v':
					os << '\\' << 'v';
					break;
				case '\\':
					os << '\\' << '\\';
					break;
				case '\'':
					os << '\\' << '\'';
					break;
				case '\"':
					os << '\\' << '\"';
					break;
				default:
					if (c > 30 && c < 127)	{
						os << (char)c;
					}
					else	{
						//	code point U+nnnn
						os
							<< "\\u"
							<< std::setw(4)
							<< std::setfill('0')
							<< c
							;
					}
					break;
				}
			}

			os << '"';

			return cs;
		}

		std::size_t serializer <std::filesystem::path, CPP>::write(std::ostream& os, std::filesystem::path const& v)	{

			calc_size const cs(os);
			os << '"' << v.string() << '"';
			return cs;
		}

		std::size_t serializer <severity, CPP>::write(std::ostream& os, severity s)	{

			calc_size const cs(os);

			os << '"';
			switch (s)
			{
			case severity::LEVEL_TRACE:		os << "cyng::make_object(cyng::severity::LEVEL_TRACE)";		break;
			case severity::LEVEL_DEBUG: 	os << "cyng::make_object(cyng::severity::LEVEL_DEBUG)";		break;
			case severity::LEVEL_INFO: 		os << "cyng::make_object(cyng::severity::LEVEL_INFO)";		break;
			case severity::LEVEL_WARNING: 	os << "cyng::make_object(cyng::severity::LEVEL_WARNING)";	break;
			case severity::LEVEL_ERROR: 	os << "cyng::make_object(cyng::severity::LEVEL_ERROR)";		break;
			case severity::LEVEL_FATAL: 	os << "cyng::make_object(cyng::severity::LEVEL_FATAL)";		break;
			default:
				break;
			}
			os << '"';

			return cs;
		}

		std::size_t serializer <vector_t, CPP>::write(std::ostream& os, vector_t const& vec)
		{
			calc_size const cs(os);

			os << "cyng::make_vector({";

			//	serialize each element from the tuple
			bool init = false;

			for (auto const& obj : vec) {
				if (!init) {
					init = true;
				}
				else {
					os << ", ";
				}
				auto const tag = obj.rtti().tag();
				switch (tag)
				{
				case TC_ATTR:
				case TC_PARAM:
					serialize_cpp(os, obj);
					break;
				default:
					serialize_cpp(os, obj);
				}
			}

			os 
				<< '}'
				<< ')'
				;

			return cs;
		}


		std::size_t serializer <tuple_t, CPP>::write(std::ostream& os, tuple_t const& tpl) {

			calc_size const cs(os);

			os << "cyng::make_tuple(";

			//	serialize each element from the tuple
			bool init = false;
			for (auto const& obj : tpl) {
				if (!init) {
					init = true;
				}
				else {
					os << ", ";
				}
				serialize_cpp(os, obj);
			}

			os << ')';

			return cs;
		}

		std::size_t serializer <param_t, CPP>::write(std::ostream& os, param_t const& param) {

			calc_size const cs(os);

			os
				<< "cyng::make_param("
				<< '"'
				<< param.first
				<< '"'
				<< ','
				<< ' '
				;

			auto const tag = param.second.rtti().tag();
			switch (tag)
			{
			case TC_ATTR:
			case TC_PARAM:
				serialize_cpp(os, param.second);
				break;
			default:
				serialize_cpp(os, param.second);
			}

			os
				<< ')'
				;

			return cs;
		}

		//std::size_t serializer <param_map_t, CPP>::write(std::ostream& os, param_map_t const& pm) {

		//	calc_size const cs(os);

		//	if (pm.empty()) {
		//		os << "null";
		//	}
		//	else {
		//		os << '{';

		//		bool flag = false;
		//		for (auto const& p : pm) {
		//			if (flag) {
		//				os << ',';
		//			}
		//			else {
		//				flag = true;
		//			}

		//			os
		//				<< '"'
		//				<< p.first
		//				<< '"'
		//				<< ':'
		//				;

		//			serialize_cpp(os, p.second);
		//		}
		//		os << '}';
		//	}

		//	return cs;
		//}

		//std::size_t serializer <buffer_t, CPP>::write(std::ostream& os, buffer_t const& buffer)
		//{
		//	calc_size const cs(os);
		//	//	store and reset stream state
		//	boost::io::ios_flags_saver  ifs(os);
		//	os << '"' << buffer << '"';

		//	return cs;
		//}

		//std::size_t serializer <std::chrono::system_clock::time_point, CPP>::write(std::ostream& os, std::chrono::system_clock::time_point const& v)	{

		//	calc_size const cs(os);
		//	std::time_t const tt = std::chrono::system_clock::to_time_t(v);

		//	//
		//	//	the generic "Jan. 1 1970" is null
		//	//
		//	if (0 == tt) {
		//		os << "null";
		//	}
		//	else {
		//		auto tm = *std::gmtime(&tt);
		//		//	example: 1995-12-04T00:12:00-0430
		//		// ISO 8601
		//		os << '"' << std::put_time(&tm, "%FT%T%z") << '"';
		//	}

		//	return cs;
		//}


	}
}
