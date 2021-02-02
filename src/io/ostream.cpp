#include <cyng/io/ostream.h>
#include <cyng/io/serialize.h>
#include <cyng/obj/object.h>

namespace docscript {

	std::ostream& operator<<(std::ostream& os, null const&)
	{
		os << "null";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, eod const&)
	{
		os << "eod";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, version const& v)
	{
		//	store and reset stream state
		boost::io::ios_flags_saver  ifs(os);

		os
			<< std::dec
			<< v.major()
			<< '.'
			<< v.minor()
			;

		return os;
	}

	std::ostream& operator<<(std::ostream& os, revision const& r)
	{
		boost::io::ios_flags_saver  ifs(os);

		const version v_maj(r.major());
		const version v_min(r.minor());

		os
			<< std::dec
			<< v_maj.major()
			<< '.'
			<< v_maj.minor()
			<< '.'
			<< v_min.major()
			<< '.'
			<< v_min.minor()
			;

		return os;
	}

	std::ostream& operator<<(std::ostream& os, op c)
	{
		os << "op:";

		switch (c)
		{
		case op::PUSH: 	os << "PUSH";	break;

		case op::NOW: 	os << "NOW";	break;
		case op::PID: 	os << "PID";	break;
		case op::TID: 	os << "TID";	break;

		//	assembly
		case op::MAKE_ATTR: 		os << "MAKE_ATTR";	break;
		case op::MAKE_PARAM: 		os << "MAKE_PARAM";	break;
		case op::MAKE_ATTR_MAP: 	os << "MAKE_ATTR_MAP";	break;
		case op::MAKE_PARAM_MAP: 	os << "MAKE_PARAM_MAP";	break;
		case op::MAKE_TUPLE: 		os << "MAKE_TUPLE";	break;
		case op::MAKE_VECTOR: 		os << "MAKE_VECTOR";	break;
		case op::MAKE_DEQUE: 		os << "MAKE_DEQUE";	break;

		case op::TIDY:	os << "TIDY";	break;

		case op::ASSERT_TYPE:	os << "ASSERT_TYPE";	break;
		case op::ASSERT_VALUE:	os << "ASSERT_VALUE";	break;

		case op::HALT:	os << "HALT";	break;

		case op::NOOP: 	os << "NOOP";	break;

		default:
			os << "CODE" << static_cast<std::uint16_t>(c)  << "!";
			break;
		}

		return os;
	}

	std::ostream& operator<<(std::ostream& os, severity s)
	{
		switch (s)
		{
		case severity::LEVEL_TRACE:		os << "TRACE";	break;
		case severity::LEVEL_DEBUG: 	os << "DEBUG";	break;
		case severity::LEVEL_INFO: 		os << "INFO ";	break;
		case severity::LEVEL_WARNING: 	os << "WARN ";	break;
		case severity::LEVEL_ERROR: 	os << "ERROR";	break;
		case severity::LEVEL_FATAL: 	os << "FATAL";	break;
		default:
			os.setstate(std::ios_base::failbit);
			break;
		}
		return os;
	}

	std::ostream& operator<<(std::ostream& os, mac48 const& v)
	{

		//	store and reset stream state
		boost::io::ios_flags_saver  ifs(os);

		os
			<< std::setfill('0')
			<< std::hex
			;

		//	std::array< std::uint8_t, 6 >
		bool initial{ true };
		for (auto const& c : v.get_octets()) {
			if (initial) {
				initial = false;
			}
			else {
				os << ':';
			}

			os
				<< std::setw(2)
				<< (+c & 0xFF)
				;
		}

		return os;
	}

	std::ostream& operator<<(std::ostream& os, mac64 const& v)
	{

		//	store and reset stream state
		boost::io::ios_flags_saver  ifs(os);

		os
			<< std::setfill('0')
			<< std::hex
			;

		//	std::array< std::uint16_t, 4 >
		bool initial{ true };
		for (auto const& word : v.get_words()) {
			if (initial) {
				initial = false;
			}
			else {
				os << ':';
			}

			os
				<< std::setw(4)
				<< word
				;
		}

		return os;
	}

	std::ostream& operator<<(std::ostream& os, pid const& p) {
		//	store and reset stream state
		boost::io::ios_flags_saver  ifs(os);

		os
			<< std::setfill('0')
			<< std::dec
			<< std::setw(6)
			<< p.operator std::uint64_t()
			;
		return os;
	}


	std::ostream& operator<<(std::ostream& os, obis const& o) {
		return os << to_str(o);
	}

	std::ostream& operator<<(std::ostream& os, buffer_t const& v)
	{
		//	store and reset stream state
		boost::io::ios_flags_saver  ifs(os);

		os
			<< std::setfill('0')
			<< std::hex
			;

		//	write buffer
		for (auto const c : v)
		{
			if (c > 31 && c < 127)
			{
				os << c;
			}
			else if (c == '\\')
			{
				os << "\\\\";
			}
			else if (c == '"')
			{
				os << "\\\"";
			}
			else if (c == '\n')
			{
				os << "\\n";
			}
			else if (c == '\t')
			{
				os << "\\t";
			}
			else
			{
				os
					<< "\\x"
					<< std::setw(2)
					<< (+c & 0xFF)
					;
			}
		}
		return os;
	}

	std::ostream& operator<<(std::ostream& os, std::chrono::system_clock::time_point const& tp)
	{
		std::time_t const tt = std::chrono::system_clock::to_time_t(tp);
		auto tm = *std::gmtime(&tt);
		return os << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S%z");
	}

	std::ostream& operator<<(std::ostream& os, tuple_t const& tpl)
	{
		os << '{';

		//	serialize each element from the tuple
		bool init = false;
		for (auto const& obj : tpl) {
			if (!init) {
				init = true;
			}
			else {
				os << ",";
			}
			io::serialize_plain(os, obj);
		}

		os << '}';
		return os;
	}

	std::ostream& operator<<(std::ostream& os, vector_t const& vec)
	{
		os << '[';

		//	serialize each element from the tuple
		bool init = false;
		for (auto const& obj : vec) {
			if (!init) {
				init = true;
			}
			else {
				os << ",";
			}
			io::serialize_plain(os, obj);
		}

		os << ']';
		return os;
	}

	std::ostream& operator<<(std::ostream& os, deque_t const& deq)
	{
		os << '<';

		//	serialize each element from the tuple
		bool init = false;
		for (auto const& obj : deq) {
			if (!init) {
				init = true;
			}
			else {
				os << ",";
			}
			io::serialize_plain(os, obj);
		}

		os << '>';
		return os;
	}

	std::ostream& operator<<(std::ostream& os, attr_map_t const& amap)
	{
		os << '#' << '(';
		bool flag = false;
		for (auto const& attr : amap) {

			if (flag) {
				os << ',';
			}
			else {
				flag = true;
			}

			os << '(' << attr.first << ':';
			io::serialize_plain(os, attr.second);
			os << ')';
		}
		os << ')';
		return os;
	}

	std::ostream& operator<<(std::ostream& os, attr_t const& attr)
	{
		boost::io::ios_flags_saver  ifs(os);

		os
			<< '('
			<< std::dec
			<< attr.first
			<< ':'
			;

		io::serialize_plain(os, attr.second);

		os
			<< ')'
			;

		return os;
	}

	std::ostream& operator<<(std::ostream& os, param_map_t const& pmap)
	{
		bool flag = false;
		os << '%' << '(';
		for (auto const& param : pmap) {

			if (flag)	{
				os << ',';
			}
			else {
				flag = true;
			}

			os << '(' << '"' << param.first << '"' << ':';
			io::serialize_plain(os, param.second);
			os << ')';
		}
		os << ')';
		return os;
	}

	std::ostream& operator<<(std::ostream& os, param_t const& param)
	{
		os
			<< '('
			<< '"'
			<< param.first
			<< '"'
			<< ':'
			;

		io::serialize_plain(os, param.second);

		os
			<< ')'
			;

		return os;
	}

	std::ostream& operator<<(std::ostream& os, object const& obj)
	{
		io::serialize_plain(os, obj);
		return os;
	}

}
