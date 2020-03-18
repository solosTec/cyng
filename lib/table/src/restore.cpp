/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Sylko Olzscher
 *
 */

#include <cyng/table/restore.h>
#include <cyng/traits.h>
#include <cyng/factory.h>
#include <cyng/parser/chrono_parser.h>
#include <cyng/parser/mac_parser.h>
#include <cyng/parser/version_parser.h>

#include <boost/algorithm/string/predicate.hpp>

namespace cyng
{
	namespace table
	{
		object restore(std::string val, std::uint32_t code)
		{
			switch (code) {
			case TC_BOOL:
				//	true is encoded as "true"
				return make_object(boost::algorithm::equals("true", val));
				//case cyng::TC_CHAR:
			case cyng::TC_FLOAT:
				return make_object(std::stof(val));
			case cyng::TC_DOUBLE:
				return make_object(std::stod(val));
			case cyng::TC_FLOAT80:
				return make_object(std::stold(val));
			case cyng::TC_UINT8:
				return make_object<std::uint8_t>(std::stoul(val));
			case cyng::TC_UINT16:
				return make_object<std::uint16_t>(std::stoul(val));
			case cyng::TC_UINT32:
				return make_object<std::uint32_t>(std::stoul(val));
			case cyng::TC_UINT64:
				return make_object<std::uint64_t>(std::stoull(val));
			case cyng::TC_INT8:
				return make_object<std::int8_t>(std::stoi(val));
			case cyng::TC_INT16:
				return make_object<std::int16_t>(std::stoi(val));
			case cyng::TC_INT32:
				return make_object<std::int32_t>(std::stoi(val));
			case cyng::TC_INT64:
				return make_object<std::int64_t>(std::stoi(val));
				//case cyng::TC_STRING:	//	default
				//case cyng::TC_TIME_POINT: 
				//case cyng::TC_NANO_SECOND:
			case cyng::TC_MICRO_SECOND:
			{
				auto const r = parse_timespan_microsec(val);
				return (r.second)
					? make_object(r.first)
					: make_object(val)
					;
			}
			break;
			//case cyng::TC_MILLI_SECOND:
			case cyng::TC_SECOND:
			{
				auto const r = parse_timespan_seconds(val);
				return (r.second)
					? make_object(r.first)
					: make_object(val)
					;
			}
			break;
			case cyng::TC_MINUTE:
			{
				auto const r = parse_timespan_minutes(val);
				return (r.second)
					? make_object(r.first)
					: make_object(val)
					;
			}
			break;
			//case cyng::TC_HOUR:

			//case cyng::TC_DBL_TP:
			case cyng::TC_VERSION:
			{
				auto const r = parse_version(val);
				return (r.second)
					? make_object(r.first)
					: make_object(val)
					;
			}
			break;
			case cyng::TC_REVISION:
			{
				auto const r = parse_revision(val);
				return (r.second)
					? make_object(r.first)
					: make_object(val)
					;
			}
			break;

			//case cyng::TC_CODE:
			//case cyng::TC_LABEL:
			//case cyng::TC_SEVERITY:
			//case cyng::TC_BUFFER:
			case cyng::TC_MAC48:
			{
				auto const r = cyng::parse_mac48(val);
				return (r.second)
					? make_object(r.first)
					: make_object(val)
					;
			}
			break;
			case cyng::TC_MAC64:
			{
				auto const r = cyng::parse_mac64(val);
				return (r.second)
					? make_object(r.first)
					: make_object(val)
					;
			}
			break;
			//case cyng::TC_COLOR_8:
			//case cyng::TC_COLOR_16:
			//case cyng::TC_IP_TCP_ENDPOINT:	//	missing parser for enpoints (address:port)
			case cyng::TC_IP_ADDRESS:
				return make_object(boost::asio::ip::make_address(val));

			default:
				break;
			}

			return make_object(val);
		}
	}

}

