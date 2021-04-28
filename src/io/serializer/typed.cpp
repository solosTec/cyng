/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#include <cyng/io/serializer/typed.hpp>
#include <cyng/io/serializer/plain.hpp>
#include <cyng/io/ostream.h>
#include <boost/io/ios_state.hpp>

namespace cyng {
	namespace io {
		std::size_t serializer <bool, TYPED>::write(std::ostream& os, bool v)
		{
			return serializer<bool, PLAIN>::write(os, v);
		}

		std::size_t serializer <version, TYPED>::write(std::ostream& os, version const& v)
		{
			calc_size const cs(os);
			os << 'v' << v;
			return cs;
		}

		std::size_t serializer <revision, TYPED>::write(std::ostream& os, revision const& r)
		{
			calc_size const cs(os);
			os << 'r' << r;
			return cs;
		}

		std::size_t serializer <std::int8_t, TYPED>::write(std::ostream& os, int8_t v)
		{
			calc_size const cs(os);
			serializer<std::int8_t, PLAIN>::write(os, v);
			os << "i8";
			return cs;
		}

		std::size_t serializer <std::int16_t, TYPED>::write(std::ostream& os, int16_t v)
		{
			serializer<std::int16_t, PLAIN>::write(os, v);
			calc_size const cs(os);
			os << "i16";
			return cs;
		}

		std::size_t serializer <std::int32_t, TYPED>::write(std::ostream& os, int32_t v)
		{
			calc_size const cs(os);
			serializer<std::int32_t, PLAIN>::write(os, v);
			os << "i32";
			return cs;
		}

		std::size_t serializer <std::int64_t, TYPED>::write(std::ostream& os, int64_t v)
		{
			calc_size const cs(os);
			serializer<std::int64_t, PLAIN>::write(os, v);
			os << "i64";
			return cs;
		}

		std::size_t serializer <std::uint8_t, TYPED>::write(std::ostream& os, uint8_t v)
		{
			calc_size const cs(os);
			serializer<std::uint8_t, PLAIN>::write(os, v);
			os << "u8";
			return cs;
		}

		std::size_t serializer <std::uint16_t, TYPED>::write(std::ostream& os, uint16_t v)
		{
			calc_size const cs(os);
			serializer<std::uint16_t, PLAIN>::write(os, v);
			os << "u16";
			return cs;
		}

		std::size_t serializer <std::uint32_t, TYPED>::write(std::ostream& os, uint32_t v)
		{
			calc_size const cs(os);
			serializer<std::uint32_t, PLAIN>::write(os, v);
			os << "u32";
			return cs;
		}

		std::size_t serializer <std::uint64_t, TYPED>::write(std::ostream& os, uint64_t v)
		{
			calc_size const cs(os);
			serializer<std::uint64_t, PLAIN>::write(os, v);
			os << "u64";
			return cs;
		}

		std::size_t serializer <float, TYPED>::write(std::ostream& os, float v)
		{
			calc_size const cs(os);
			serializer<float, PLAIN>::write(os, v);
			os << "f";
			return cs;
		}

		std::size_t serializer <double, TYPED>::write(std::ostream& os, double v)
		{
			calc_size const cs(os);
			serializer<double, PLAIN>::write(os, v);
			os << "d";
			return cs;
		}

		std::size_t serializer <std::chrono::system_clock::time_point, TYPED>::write(std::ostream& os, std::chrono::system_clock::time_point const& tp)
		{
			static const type<std::chrono::system_clock::time_point> type;

			calc_size const cs(os);
			os << '"' << tp << '"' << type.type_name();
			return cs;
		}

		std::size_t serializer <std::string, TYPED>::write(std::ostream& os, std::string const& s)
		{
			static const type<std::string> type;

			calc_size const cs(os);
			os << '"' << s << '"' << type.type_name();
			return cs;
		}

		std::size_t serializer <std::filesystem::path, TYPED>::write(std::ostream& os, std::filesystem::path const& p)
		{
			static const type<std::filesystem::path> type;

			calc_size const cs(os);
			os << '"' << p.generic_string() << '"' << type.type_name();
			return cs;
		}

		std::size_t serializer <null, TYPED>::write(std::ostream& os, null n)
		{
			calc_size const cs(os);
			os << n;
			return cs;
		}

		std::size_t serializer <eod, TYPED>::write(std::ostream& os, eod e)
		{
			calc_size const cs(os);
			os << e;
			return cs;
		}

		std::size_t serializer <op, TYPED>::write(std::ostream& os, op code)
		{
			calc_size const cs(os);
			os << code;
			return cs;
		}

		std::size_t serializer <severity, TYPED>::write(std::ostream& os, severity level)
		{
			calc_size const cs(os);
			os << '!';
			switch (level)
			{
			case severity::LEVEL_TRACE:		os << "TRACE";	break;
			case severity::LEVEL_DEBUG: 	os << "DEBUG";	break;
			case severity::LEVEL_INFO: 		os << "INFO";	break;
			case severity::LEVEL_WARNING: 	os << "WARN";	break;
			case severity::LEVEL_ERROR: 	os << "ERROR";	break;
			case severity::LEVEL_FATAL: 	os << "FATAL";	break;
			default:
				os << "!";
				break;
			}
			return cs;
		}

	}
}
