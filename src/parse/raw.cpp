/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#include <cyng/parse/raw.h>
#include <cyng/parse/string.h>
#include <cyng/parse/mac.h>
#include <cyng/parse/version.h>
#include <cyng/parse/color.h>
#include <cyng/parse/buffer.h>
#include <cyng/parse/duration.h>
#include <cyng/parse/op.h>

#include <cyng/obj/factory.hpp>
#include <cyng/obj/tag.hpp>

#include <algorithm>

#include <boost/assert.hpp>
#include <boost/algorithm/string.hpp>

namespace cyng {

	bool is_constructable(std::uint16_t tc) {
		switch (tc) {
		case TC_NULL:
		case TC_BOOL:
			return true;

		case TC_CHAR:
			break;

		case TC_FLOAT:
		case TC_DOUBLE:
		case TC_FLOAT80:
		case TC_UINT8:
		case TC_UINT16:
		case TC_UINT32:
		case TC_UINT64:
		case TC_INT8:
		case TC_INT16:
		case TC_INT32:
		case TC_INT64:
		case TC_STRING:
			return true;

		case TC_FS_PATH:
			break;

		case TC_TIME_POINT:
		case TC_NANO_SECOND:
		case TC_MICRO_SECOND:
		case TC_MILLI_SECOND:
		case TC_SECOND:
		case TC_MINUTE:
		case TC_HOUR:
		case TC_BUFFER:
		case TC_VERSION:
		case TC_REVISION:
		case TC_OP:
			return true;
		case TC_SEVERITY:
			break;
		case TC_MAC48:
		case TC_MAC64:
			return true;

		case TC_PID:
		case TC_OBIS:
		case TC_OBISPATH:
		case TC_EDIS:
			break;

		case TC_COLOR_8:
		case TC_COLOR_16:
		case TC_DIGEST_MD5:
		case TC_DIGEST_SHA1:
		case TC_DIGEST_SHA256:
		case TC_DIGEST_SHA512:
		case TC_AES128:
		case TC_AES192:
		case TC_AES256:
			return true;

		case TC_OBJECT:
		case TC_RAW:
			//	nested
			break;

		case TC_TUPLE:
		case TC_VECTOR:
		case TC_DEQUE:

		case TC_ATTR_MAP:
		case TC_ATTR:
		case TC_PARAM_MAP:
		case TC_PARAM:
			break;
		case TC_EC:
			break;
		case TC_UUID:
		case TC_IP_ADDRESS:
		case TC_IP_TCP_ENDPOINT:
		case TC_IP_UDP_ENDPOINT:
		case TC_IP_ICMP_ENDPOINT:
		case TC_EOD:
			return true;

		default:
			break;
		}
		return false;

	}

	object construct_object(raw const& r) {
		switch (r.get_code()) {
		case TC_NULL:
			//	no literal required
			break;

		case TC_BOOL:
			return make_object(boost::algorithm::equals(r.get_literal(), "true"));
		case TC_CHAR:
			break;

		case TC_FLOAT:
			return make_object(to_numeric<traits::reverse_type<TC_FLOAT>::type>(r.get_literal()));
		case TC_DOUBLE:
			return make_object(to_numeric<traits::reverse_type<TC_DOUBLE>::type>(r.get_literal()));
		case TC_FLOAT80:
			return make_object(to_numeric<traits::reverse_type<TC_FLOAT80>::type>(r.get_literal()));

		case TC_UINT8:	
			return make_object(to_numeric<traits::reverse_type<TC_UINT8>::type>(r.get_literal()));
		case TC_UINT16:
			return make_object(to_numeric<traits::reverse_type<TC_UINT16>::type>(r.get_literal()));
		case TC_UINT32:
			return make_object(to_numeric<traits::reverse_type<TC_UINT32>::type>(r.get_literal()));
		case TC_UINT64:
			return make_object(to_numeric<traits::reverse_type<TC_UINT64>::type>(r.get_literal()));
		case TC_INT8:
			return make_object(to_numeric<traits::reverse_type<TC_INT8>::type>(r.get_literal()));
		case TC_INT16:
			return make_object(to_numeric<traits::reverse_type<TC_INT16>::type>(r.get_literal()));
		case TC_INT32:
			return make_object(to_numeric<traits::reverse_type<TC_INT32>::type>(r.get_literal()));
		case TC_INT64:
			return make_object(to_numeric<traits::reverse_type<TC_INT64>::type>(r.get_literal()));

		case TC_STRING:	
			return make_object(r.get_literal());
		case TC_FS_PATH:
			break;

		case TC_TIME_POINT:
			return make_object(to_tp_iso8601(r.get_literal()));
		case TC_NANO_SECOND:
			return make_object(to_nanoseconds(r.get_literal()));
		case TC_MICRO_SECOND:
			return make_object(to_microseconds(r.get_literal()));
		case TC_MILLI_SECOND:
			return make_object(to_milliseconds(r.get_literal()));
		case TC_SECOND:
			return make_object(to_seconds(r.get_literal()));
		case TC_MINUTE:
			return make_object(to_minutes(r.get_literal()));
		case TC_HOUR:
			return make_object(to_hours(r.get_literal()));

		case TC_BUFFER:
			return make_object(hex_to_buffer(r.get_literal()));
		case TC_VERSION:
			return make_object(to_version(r.get_literal()));
		case TC_REVISION:
			return make_object(to_revision(r.get_literal()));
		case TC_OP:
			return make_object(to_op(r.get_literal()));
		case TC_SEVERITY:
			break;
		case TC_MAC48:
			return make_object(to_mac48(r.get_literal()));
		case TC_MAC64:
			return make_object(to_mac64(r.get_literal()));
		case TC_PID:
		case TC_OBIS:
		case TC_OBISPATH:
		case TC_EDIS:
			break;
		case TC_COLOR_8:
			return make_object(to_color<std::uint8_t>(r.get_literal()));
		case TC_COLOR_16:
			return make_object(to_color<std::uint16_t>(r.get_literal()));

		case TC_DIGEST_MD5:
			return make_object(to_digest<crypto::md5_size>(r.get_literal()));
		case TC_DIGEST_SHA1:
			return make_object(to_digest<crypto::sha1_size>(r.get_literal()));
		case TC_DIGEST_SHA256:
			return make_object(to_digest<crypto::sha256_size>(r.get_literal()));
		case TC_DIGEST_SHA512:
			return make_object(to_digest<crypto::sha512_size>(r.get_literal()));

		case TC_AES128:
			return make_object(to_aes_key<crypto::aes128_size>(r.get_literal()));
		case TC_AES192:
			return make_object(to_aes_key<crypto::aes192_size>(r.get_literal()));
		case TC_AES256:
			return make_object(to_aes_key<crypto::aes256_size>(r.get_literal()));

		case TC_OBJECT:
		case TC_RAW:	
			//	nested
			break;

		case TC_TUPLE:
		case TC_VECTOR:
		case TC_DEQUE:

		case TC_ATTR_MAP:
		case TC_ATTR:
		case TC_PARAM_MAP:
		case TC_PARAM:
			break;
		case TC_EC:
			break;
		case TC_UUID:
			return make_object(to_uuid(r.get_literal()));
		case TC_IP_ADDRESS:
			return make_object(to_ip_address(r.get_literal()));
		case TC_IP_TCP_ENDPOINT:
			return make_object(to_ip_endpoint<boost::asio::ip::tcp>(r.get_literal()));
		case TC_IP_UDP_ENDPOINT:
			return make_object(to_ip_endpoint<boost::asio::ip::udp>(r.get_literal()));
		case TC_IP_ICMP_ENDPOINT:
			return make_object(to_ip_endpoint<boost::asio::ip::icmp>(r.get_literal()));

		case TC_EOD:	
			//	no literal required
			return make_eod();

		default:
			break;
		}
		return make_object();
	}
}

