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

namespace docscript {
	namespace io {
		std::size_t serializer <bool, TYPED>::write(std::ostream& os, bool v)
		{
			return serializer<bool, PLAIN>::write(os, v);
		}

		std::size_t serializer <version, TYPED>::write(std::ostream& os, version const& v)
		{
			auto const pos = os.tellp();
			os << 'v' << v;
			return os.tellp() - pos;
		}

		std::size_t serializer <revision, TYPED>::write(std::ostream& os, revision const& r)
		{
			auto const pos = os.tellp();
			os << 'r' << r;
			return os.tellp() - pos;
		}

		std::size_t serializer <std::int8_t, TYPED>::write(std::ostream& os, int8_t v)
		{
			auto const pos = os.tellp();
			serializer<std::int8_t, PLAIN>::write(os, v);
			os << "i8";
			return os.tellp() - pos;
		}

		std::size_t serializer <std::int16_t, TYPED>::write(std::ostream& os, int16_t v)
		{
			serializer<std::int16_t, PLAIN>::write(os, v);
			auto const pos = os.tellp();
			os << "i16";
			return os.tellp() - pos;
		}

		std::size_t serializer <std::int32_t, TYPED>::write(std::ostream& os, int32_t v)
		{
			auto const pos = os.tellp();
			serializer<std::int32_t, PLAIN>::write(os, v);
			os << "i32";
			return os.tellp() - pos;
		}

		std::size_t serializer <std::int64_t, TYPED>::write(std::ostream& os, int64_t v)
		{
			auto const pos = os.tellp();
			serializer<std::int64_t, PLAIN>::write(os, v);
			os << "i64";
			return os.tellp() - pos;
		}

		std::size_t serializer <std::uint8_t, TYPED>::write(std::ostream& os, uint8_t v)
		{
			auto const pos = os.tellp();
			serializer<std::uint8_t, PLAIN>::write(os, v);
			os << "u8";
			return os.tellp() - pos;
		}

		std::size_t serializer <std::uint16_t, TYPED>::write(std::ostream& os, uint16_t v)
		{
			auto const pos = os.tellp();
			serializer<std::uint16_t, PLAIN>::write(os, v);
			os << "u16";
			return os.tellp() - pos;
		}

		std::size_t serializer <std::uint32_t, TYPED>::write(std::ostream& os, uint32_t v)
		{
			auto const pos = os.tellp();
			serializer<std::uint32_t, PLAIN>::write(os, v);
			os << "u32";
			return os.tellp() - pos;
		}

		std::size_t serializer <std::uint64_t, TYPED>::write(std::ostream& os, uint64_t v)
		{
			auto const pos = os.tellp();
			serializer<std::uint64_t, PLAIN>::write(os, v);
			os << "u64";
			return os.tellp() - pos;
		}

		std::size_t serializer <float, TYPED>::write(std::ostream& os, float v)
		{
			auto const pos = os.tellp();
			serializer<float, PLAIN>::write(os, v);
			os << "f";
			return os.tellp() - pos;
		}

		std::size_t serializer <double, TYPED>::write(std::ostream& os, double v)
		{
			auto const pos = os.tellp();
			serializer<double, PLAIN>::write(os, v);
			os << "d";
			return os.tellp() - pos;
		}

		std::size_t serializer <std::chrono::system_clock::time_point, TYPED>::write(std::ostream& os, std::chrono::system_clock::time_point const& tp)
		{
			static const type<std::chrono::system_clock::time_point> type;

			auto const pos = os.tellp();
			os << '"' << tp << '"' << type.type_name();
			return os.tellp() - pos;
		}

		std::size_t serializer <std::string, TYPED>::write(std::ostream& os, std::string const& s)
		{
			static const type<std::string> type;

			auto const pos = os.tellp();
			os << '"' << s << '"' << type.type_name();
			return os.tellp() - pos;
		}

		std::size_t serializer <std::filesystem::path, TYPED>::write(std::ostream& os, std::filesystem::path const& p)
		{
			static const type<std::filesystem::path> type;

			auto const pos = os.tellp();
			os << '"' << p.generic_string() << '"' << type.type_name();
			return os.tellp() - pos;
		}

		std::size_t serializer <null, TYPED>::write(std::ostream& os, null n)
		{
			auto const pos = os.tellp();
			os << n;
			return os.tellp() - pos;
		}

		std::size_t serializer <eod, TYPED>::write(std::ostream& os, eod e)
		{
			auto const pos = os.tellp();
			os << e;
			return os.tellp() - pos;
		}

		std::size_t serializer <op, TYPED>::write(std::ostream& os, op code)
		{
			auto const pos = os.tellp();
			os << code;
			return os.tellp() - pos;
		}

		std::size_t serializer <severity, TYPED>::write(std::ostream& os, severity level)
		{
			auto const pos = os.tellp();
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
			return os.tellp() - pos;
		}

	}
}
