/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <cyng/io/parser/parser.h>
#include <cyng/obj/util.hpp>

#include <boost/assert.hpp>

namespace cyng {
	namespace io {

		parser::parser(callback cb)
			: cb_(cb)
			, state_(state::TYPE)
			, buffer_()
			, type_(0)
			, length_(0)
		{
			BOOST_ASSERT(cb);
		}

		void parser::put(char c)
		{
			switch (state_) {

			case state::FAILURE:
				BOOST_ASSERT(state_ != state::FAILURE);
				break;

			case state::TYPE:
				state_ = type(c);
				break;
			case state::LENGTH:
				state_ = length(c);
				break;
			case state::VALUE:
				state_ = value(c);
				break;

			default:
				BOOST_ASSERT(false);
				state_ = state::FAILURE;
				break;
			}
		}

		parser::state parser::type(char c)
		{
			buffer_.push_back(c);
			if (sizeof(std::uint16_t) == buffer_.size()) {

				//if constexpr (std::endian::native == std::endian::little) {
				//	std::reverse(buffer_.begin(), buffer_.end());
				//}

				//
				//	complete
				//
				type_ = to_numeric<std::uint16_t>(buffer_);
				buffer_.clear();
				length_ = 0;

				//
				//	update parser state
				//
				return state::LENGTH;
			}
			return state_;
		}

		parser::state parser::length(char c)
		{
			buffer_.push_back(c);
			BOOST_ASSERT(!buffer_.empty());
			auto const length = size_of_length_field(buffer_.at(0));
			if (length == buffer_.size()) {

				//
				//	complete
				//
				set_length(length);
				if (length_ == 0) {
					cb_(deserialize());
					return  state::TYPE;
				}
				return state::VALUE;

			}
			return state_;
		}

		void parser::set_length(std::size_t length)
		{
			switch (length) {
			case 1:
				length_ = buffer_.at(0);
				buffer_.clear();
				buffer_.reserve(length_);
				break;
			case 3:
				//	size is send in network byte order
				length_ = to_numeric<std::uint16_t>(buffer_, 1);
				buffer_.clear();
				buffer_.reserve(length_);
				break;
			default:
				BOOST_ASSERT(length == 9);
				//	size is send in network byte order
				length_ = to_numeric<std::uint64_t>(buffer_, 1);
				buffer_.clear();
				buffer_.reserve(length_);
				break;
			}
		}

		parser::state parser::value(char c)
		{
			buffer_.push_back(c);
			
			if (--length_ == 0) {

				//
				//	complete
				//
				cb_(deserialize());
				buffer_.clear();
				return state::TYPE;
			}
			return state_;
		}

		object parser::deserialize()
		{
			switch (type_) {
			case TC_NULL:	
				BOOST_ASSERT(buffer_.empty());
				return make_object();

			case TC_BOOL:		
				BOOST_ASSERT(buffer_.size() == 1);
				return make_object(buffer_.at(0) == 1);
			case TC_CHAR:		
				BOOST_ASSERT(buffer_.size() == 1);
				return make_object(buffer_.at(0));
			case TC_FLOAT:		return deserialize_numeric<traits::reverse_type<TC_FLOAT>::type>();
			case TC_DOUBLE:		return deserialize_numeric<traits::reverse_type<TC_DOUBLE>::type>();
			case TC_FLOAT80:	return deserialize_numeric<traits::reverse_type<TC_FLOAT80>::type>();

			case TC_UINT8:	return deserialize_numeric<traits::reverse_type<TC_UINT8>::type>();
			case TC_UINT16:	return deserialize_numeric<traits::reverse_type<TC_UINT16>::type>();
			case TC_UINT32:	return deserialize_numeric<traits::reverse_type<TC_UINT32>::type>();
			case TC_UINT64:	return deserialize_numeric<traits::reverse_type<TC_UINT64>::type>();
			case TC_INT8:	return deserialize_numeric<traits::reverse_type<TC_INT8>::type>();
			case TC_INT16:	return deserialize_numeric<traits::reverse_type<TC_INT16>::type>();
			case TC_INT32:	return deserialize_numeric<traits::reverse_type<TC_INT32>::type>();
			case TC_INT64:	return deserialize_numeric<traits::reverse_type<TC_INT64>::type>();

			case TC_STRING:	return make_object(make_string(buffer_));

			case TC_FS_PATH:	return make_object(make_fs_path(buffer_));

			case TC_TIME_POINT:		return make_object(make_timepoint(buffer_));
			case TC_NANO_SECOND:	return make_object(make_duration<traits::reverse_type<TC_NANO_SECOND>::type>(buffer_));
			case TC_MICRO_SECOND:	return make_object(make_duration<traits::reverse_type<TC_MICRO_SECOND>::type>(buffer_));
			case TC_MILLI_SECOND:	return make_object(make_duration<traits::reverse_type<TC_MILLI_SECOND>::type>(buffer_));
			case TC_SECOND:			return make_object(make_duration<traits::reverse_type<TC_SECOND>::type>(buffer_));
			case TC_MINUTE:			return make_object(make_duration<traits::reverse_type<TC_MINUTE>::type>(buffer_));
			case TC_HOUR:			return make_object(make_duration<traits::reverse_type<TC_HOUR>::type>(buffer_));

			case TC_BUFFER:		return make_object(buffer_);

			case TC_VERSION:	return make_object(make_version(buffer_));
			case TC_REVISION:	return make_object(make_revision(buffer_));
			case TC_OP:			return make_object(make_op(buffer_));
			case TC_SEVERITY:	return make_object(make_severity(buffer_));
			case TC_MAC48:	
				BOOST_ASSERT(buffer_.size() == mac48::size());
				return make_object(make_mac48(buffer_));
			case TC_MAC64:	
				BOOST_ASSERT(buffer_.size() == mac64::size());
				return make_object(make_mac64(buffer_));
			case TC_PID:
				return make_object(make_pid(buffer_));

			case TC_OBIS:
				BOOST_ASSERT(buffer_.size() == obis::size());
				return make_object(make_obis(buffer_));
			case TC_EDIS:
				BOOST_ASSERT(buffer_.size() == edis::size());
				return make_object(make_edis(buffer_));

			case TC_DIGEST_MD5:	
				BOOST_ASSERT(buffer_.size() == crypto::digest_md5::size());
				return make_object(make_digest<crypto::digest_md5::size()>(buffer_));
			case TC_DIGEST_SHA1:	
				BOOST_ASSERT(buffer_.size() == crypto::digest_sha1::size());
				return make_object(make_digest<crypto::digest_sha1::size()>(buffer_));
			case TC_DIGEST_SHA256:	
				BOOST_ASSERT(buffer_.size() == crypto::digest_sha256::size());
				return make_object(make_digest<crypto::digest_sha256::size()>(buffer_));
			case TC_DIGEST_SHA512:
				BOOST_ASSERT(buffer_.size() == crypto::digest_sha512::size());
				return make_object(make_digest<crypto::digest_sha512::size()>(buffer_));

			case TC_AES128:	
				BOOST_ASSERT(buffer_.size() == crypto::aes_128_key::bytes());
				return make_object(make_aes_key<crypto::aes_128_key::size()>(buffer_));
			case TC_AES192:
				BOOST_ASSERT(buffer_.size() == crypto::aes_192_key::bytes());
				return make_object(make_aes_key<crypto::aes_192_key::size()>(buffer_));
			case TC_AES256:
				BOOST_ASSERT(buffer_.size() == crypto::aes_256_key::bytes());
				return make_object(make_aes_key<crypto::aes_256_key::size()>(buffer_));

			case TC_TUPLE:	
			case TC_VECTOR:	
			case TC_DEQUE:	

			case TC_ATTR_MAP:	
			case TC_ATTR:		
			case TC_PARAM_MAP:	
			case TC_PARAM:
				//	these data types will be not serialized directly but by
				//	build instructions
				BOOST_ASSERT_MSG(false, "wrong serialization sequence");
				break;

			case TC_EC:		return make_object(make_ec(buffer_));
			case TC_UUID:	return make_object(make_uuid(buffer_));

			case TC_IP_ADDRESS:	return make_object(make_ip_address(buffer_));

			case TC_IP_TCP_ENDPOINT:	return make_object(make_endpoint<traits::reverse_type<TC_IP_TCP_ENDPOINT>::type>(buffer_));
			case TC_IP_UDP_ENDPOINT:	return make_object(make_endpoint<traits::reverse_type<TC_IP_UDP_ENDPOINT>::type>(buffer_));
			case TC_IP_ICMP_ENDPOINT:	return make_object(make_endpoint<traits::reverse_type<TC_IP_ICMP_ENDPOINT>::type>(buffer_));

			case TC_EOD:	
				BOOST_ASSERT(buffer_.empty());
				return make_eod();

			default:
				BOOST_ASSERT_MSG(false, "unknown data type");
				break;
			}
			return make_object(buffer_);
		}

	}
}

