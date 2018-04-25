/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/io/serializer/binary.hpp>
#include <cyng/io/serializer.h>
#include <cyng/factory.h>
#include <cyng/object.h>
#include <algorithm>
#include <boost/io/ios_state.hpp>

namespace cyng 
{	
	namespace io
	{
		
		std::ostream& serializer <tuple_t, SERIALIZE_BINARY>::write(std::ostream& os, tuple_t const& v)
		{
			//	serialize each element from the tuple
			std::for_each(v.begin(), v.end(), [&os](object const& obj){
				serialize_binary(os, obj);
			});

			//
			//	serialize tuple size
			//	
			write_size_as_object(os, v.size());
			
			//
			//	serialize instruction to build a tuple
			//
			serialize_binary(os, make_object(code::ASSEMBLE_TUPLE));
			return os;
		}
		
		std::ostream& serializer <vector_t, SERIALIZE_BINARY>::write(std::ostream& os, vector_t const& v)
		{
			//	serialize each element from vector
			std::for_each(v.begin(), v.end(), [&os](object const& obj){
				serialize_binary(os, obj);
			});

			//
			//	serialize vector size
			//	
			write_size_as_object(os, v.size());

			//
			//	serialize instruction to build a vector
			//	ToDo: define VM op codes
			//
			serialize_binary(os, make_object(code::ASSEMBLE_VECTOR));
			return os;
			
		}

		std::ostream& serializer <set_t, SERIALIZE_BINARY>::write(std::ostream& os, set_t const& v)
		{
			//	serialize each element from set
			std::for_each(v.begin(), v.end(), [&os](object const& obj){
				serialize_binary(os, obj);
			});

			//
			//	serialize set size
			//	
			write_size_as_object(os, v.size());

			//
			//	serialize instruction to build a set
			//	ToDo: define VM op codes
			//
			serialize_binary(os, make_object(code::ASSEMBLE_SET));
			return os;
		}
		
		std::ostream& serializer <bool, SERIALIZE_BINARY>::write(std::ostream& os, bool v)
		{
			//
			//	type
			//
			serialize_type_tag<bool>(os);
			const std::uint8_t b = v ? 1 : 0;

			//
			//	length
			//
			serialize_length(os, sizeof(b));
			
			//
			//	value
			//
 			write_binary(os, b);
			return os;
		}
		
		std::ostream& serializer<null, SERIALIZE_BINARY>::write(std::ostream& os, null)
		{
			//
			//	type - length - no data
			//
			serialize_type_tag<null>(os);
			serialize_length(os, 0);
			return os;
		}
		
		std::ostream& serializer<std::string, SERIALIZE_BINARY>::write(std::ostream& os, std::string const& v)
		{
			//
			//	type - length - data
			//
			serialize_type_tag<std::string>(os);
			serialize_length(os, v.size());
			if (!v.empty())
			{
				os.write(v.c_str(), v.size());
			}
			return os;
		}
		
		std::ostream& serializer<std::chrono::system_clock::time_point, SERIALIZE_BINARY>::write(std::ostream& os, std::chrono::system_clock::time_point const& v)
		{
			using duration = std::chrono::system_clock::time_point::duration;
			const auto diff = std::chrono::duration_cast<duration>(v - std::chrono::system_clock::time_point::min());

			//
			//	Write a time point value in binary format as a difference.
			//	type - length - diff
			//
			serialize_type_tag<std::chrono::system_clock::time_point>(os);
			serialize_length(os, sizeof(diff));
			write_binary(os, diff.count());
			return os;
		}

		std::ostream& serializer <version, SERIALIZE_BINARY>::write(std::ostream& os, version const& v)
		{
			//
			//	type - length - value
			//
			serialize_type_tag<version>(os);
			serialize_length(os, sizeof(v.full()));
			write_binary(os, v.full());
			return os;
		}
		
		std::ostream& serializer <revision, SERIALIZE_BINARY>::write(std::ostream& os, revision const& v)
		{
			//
			//	type - length - value
			//
			serialize_type_tag<revision>(os);
			serialize_length(os, sizeof(v.full()));
			write_binary(os, v.full());
			return os;
		}
		
		std::ostream& serializer <char, SERIALIZE_BINARY>::write(std::ostream& os, char const& v)
		{
			//
			//	type - length - value
			//
			serialize_type_tag<std::string>(os);
			serialize_length(os, sizeof(v));
			write_binary(os, v);
			return os;
		}
		
		std::ostream& serializer <mac48, SERIALIZE_BINARY>::write(std::ostream& os, mac48 const& v)
		{
			static_assert(sizeof(mac48::address_type) == 6, "invalid assumption");			
			
			//
			//	type - length - value
			//
			serialize_type_tag<mac48>(os);
			serialize_length(os, std::tuple_size<mac48::address_type>::value);
			write_binary(os, v.get_octets());
			return os;
		}
		
		std::ostream&  serializer <mac64, SERIALIZE_BINARY>::write(std::ostream& os, mac64 const& v)
		{
			static_assert(sizeof(mac64::address_type) == 8, "invalid assumption");			
			
			//
			//	type - length - value
			//
			serialize_type_tag<mac64>(os);
			serialize_length(os, std::tuple_size<mac64::address_type>::value);
			write_binary(os, v.get_words());
			return os;
		}
		
		std::ostream& serializer <boost::system::error_code, SERIALIZE_BINARY>::write(std::ostream& os, boost::system::error_code const& v)
		{
			const auto code = v.value();

			//
			//	type - length - value
			//
			serialize_type_tag<boost::system::error_code>(os);
			serialize_length(os, sizeof(code));
			write_binary(os, code);
			return os;
		}

		std::ostream& serializer <boost::uuids::uuid, SERIALIZE_BINARY>::write(std::ostream& os, boost::uuids::uuid const& v)
		{
			static_assert(sizeof(v) == 16, "invalid assumption");

			//
			//	type - length - value
			//
			serialize_type_tag<boost::uuids::uuid>(os);
			serialize_length(os, sizeof(v));
 			os.write(reinterpret_cast<const std::ostream::char_type*>(v.data), sizeof(v));
			return os;
		}
		
		std::ostream& serializer <boost::filesystem::path, SERIALIZE_BINARY>::write(std::ostream& os, boost::filesystem::path const& v)
		{
			//
			//	type - length - value
			//
			serialize_type_tag<boost::filesystem::path>(os);
			serialize_length(os, v.size());
			return os << v.string();
		}
		
		std::ostream& serializer <eod, SERIALIZE_BINARY>::write(std::ostream& os, eod)
		{
			//
			//	type - length - no data
			//
			serialize_type_tag<eod>(os);
			serialize_length(os, 0);
			return os;
		};
		
		std::ostream& serializer <cyng::chrono::dbl_time_point, SERIALIZE_BINARY>::write(std::ostream& os, cyng::chrono::dbl_time_point const& v)
		{
			//	std::pair<std::time_t, double>
			static_assert(sizeof(chrono::dbl_time_point) == sizeof(std::time_t) + sizeof(double), "alignment error");

			//
			//	type - length - values
			//
			serialize_type_tag<chrono::dbl_time_point>(os);
			serialize_length(os, sizeof(chrono::dbl_time_point));
			write_binary(os, v.first);
			write_binary(os, v.second);
			return os;
		};

		std::ostream& serializer <buffer_t, SERIALIZE_BINARY>::write(std::ostream& os, buffer_t const& v)
		{
			//
			//	type - length - value
			//
			serialize_type_tag<buffer_t>(os);
			serialize_length(os, v.size());
			os.write(v.data(), v.size());
			return os;
		};

		std::ostream& serializer <color_8, SERIALIZE_BINARY>::write(std::ostream& os, color_8 const& v)
		{
			//rgb_type = std::array< T, DIM >;
			serialize_type_tag<color_8>(os);
			serialize_length(os, sizeof(color_8::rgb_type));
			write_binary(os, v.red());
			write_binary(os, v.green());
			write_binary(os, v.blue());
			write_binary(os, v.opacity());
			return os;
		};

		std::ostream& serializer <color_16, SERIALIZE_BINARY>::write(std::ostream& os, color_16 const& v)
		{
			//rgb_type = std::array< T, DIM >;
			serialize_type_tag<color_16>(os);
			serialize_length(os, sizeof(color_16::rgb_type));
			write_binary(os, v.red());
			write_binary(os, v.green());
			write_binary(os, v.blue());
			write_binary(os, v.opacity());
			return os;
		};

		std::ostream& serializer <attr_t, SERIALIZE_BINARY>::write(std::ostream& os, attr_t const& v)
		{
			//	serialize value
			serialize_binary(os, v.second);

			//	serialize index
			write_size_as_object(os, v.first);

			//	serialize instruction
			serialize_binary(os, make_object(code::ASSEMBLE_ATTR));
			return os;
		};

		std::ostream& serializer <attr_map_t, SERIALIZE_BINARY>::write(std::ostream& os, attr_map_t const& v)
		{
			//	serialize each element from attribute map
			for (attr_map_t::value_type const& attr : v)
			{
				serializer <attr_t, SERIALIZE_BINARY>::write(os, attr);
			}

			//	element count
			write_size_as_object(os, v.size());

			//	serialize instruction
			serialize_binary(os, make_object(code::ASSEMBLE_ATTR_MAP));
			return os;
		}

		std::ostream& serializer <param_map_t, SERIALIZE_BINARY>::write(std::ostream& os, param_map_t const& v)
		{
			//	serialize each element from parameter map
			for (param_map_t::value_type const& param : v)
			{
				serializer <param_t, SERIALIZE_BINARY>::write(os, param);
			}

			//	element count
			write_size_as_object(os, v.size());

			//	serialize instruction
			serialize_binary(os, make_object(code::ASSEMBLE_PARAM_MAP));
			return os;
		}


		std::ostream& serializer <param_t, SERIALIZE_BINARY>::write(std::ostream& os, param_t const& v)
		{
			//	serialize value
			serialize_binary(os, v.second);

			//	serialize name
			serialize_binary(os, make_object(v.first));

			//	serialize instruction
			serialize_binary(os, make_object(code::ASSEMBLE_PARAM));

			return os;
		}

		
		std::ostream& serializer <boost::asio::ip::tcp::endpoint, SERIALIZE_BINARY>::write(std::ostream& os, boost::asio::ip::tcp::endpoint const& v)
		{
			const auto address = v.address().to_string();
			const auto port = v.port();

			//
			//	type - length - data
			//
			serialize_type_tag<boost::asio::ip::tcp::endpoint>(os);
			serialize_length(os, address.size() + sizeof(port));
			write_binary(os, v.port());
			return os << address;
		}
		
		std::ostream& serializer <boost::asio::ip::udp::endpoint, SERIALIZE_BINARY>::write(std::ostream& os, boost::asio::ip::udp::endpoint const& v)
		{
			const auto address = v.address().to_string();
			const auto port = v.port();

			//
			//	type - length - data
			//
			serialize_type_tag<boost::asio::ip::udp::endpoint>(os);
			serialize_length(os, address.size() + sizeof(port));
			write_binary(os, v.port());
			return os << address;
		}

		std::ostream& serializer <boost::asio::ip::icmp::endpoint, SERIALIZE_BINARY>::write(std::ostream& os, boost::asio::ip::icmp::endpoint const& v)
		{
			const auto address = v.address().to_string();
			const auto port = v.port();

			//
			//	type - length - data
			//
			serialize_type_tag<boost::asio::ip::icmp::endpoint>(os);
			serialize_length(os, address.size() + sizeof(port));
			write_binary(os, v.port());
			return os << address;
		}

		
		std::ostream& serializer <boost::asio::ip::address, SERIALIZE_BINARY>::write(std::ostream& os, boost::asio::ip::address const& v)
		{
			const std::string s = v.to_string();

			//
			//	type - length - data
			//
			serialize_type_tag<boost::asio::ip::address>(os);
			serialize_length(os, s.size());
			return os << v;
		}

		
		std::ostream& serializer <crypto::digest_md5, SERIALIZE_BINARY>::write(std::ostream& os, crypto::digest_md5 const& digest)
		{
			serialize_type_tag<crypto::digest_md5>(os);
			serialize_length(os, sizeof(crypto::digest_md5::value_type));
			write_binary(os, digest.data_);
			return os;
		}
		std::ostream& serializer <crypto::digest_sha1, SERIALIZE_BINARY>::write(std::ostream& os, crypto::digest_sha1 const& digest)
		{
			serialize_type_tag<crypto::digest_sha1>(os);
			serialize_length(os, sizeof(crypto::digest_sha1::value_type));
			write_binary(os, digest.data_);
			return os;
		}
		std::ostream& serializer <crypto::digest_sha256, SERIALIZE_BINARY>::write(std::ostream& os, crypto::digest_sha256 const& digest)
		{
			serialize_type_tag<crypto::digest_sha256>(os);
			serialize_length(os, sizeof(crypto::digest_sha256::value_type));
			write_binary(os, digest.data_);
			return os;
		}
		
		std::ostream& serializer <crypto::digest_sha512, SERIALIZE_BINARY>::write(std::ostream& os, crypto::digest_sha512 const& digest)
		{
			serialize_type_tag<crypto::digest_sha512>(os);
			serialize_length(os, sizeof(crypto::digest_sha512::value_type));
			write_binary(os, digest.data_);
			return os;
		}
		
		std::size_t serialize_length(std::ostream& os, std::size_t length)
		{
			if (length < 126)
			{
				write_binary(os, std::uint8_t(length & 0xFF));
				return sizeof(std::uint8_t);
			}
			else if (length < 0xFFFF)
			{
				write_binary(os, std::uint8_t(126));
				write_binary(os, std::uint16_t(length & 0xFFFF));
				return sizeof(std::uint8_t) + sizeof(std::uint16_t);
			}

			write_binary(os, std::uint8_t(127));
			write_binary(os, std::uint64_t(length));
			return sizeof(std::uint8_t) + sizeof(std::uint64_t);
	
		}
		
		void write_size_as_object(std::ostream& os, std::size_t v)
		{
			//
			//	serialize std::size_t as object
			//	
			try {
				const std::uint64_t size = boost::numeric_cast<std::uint64_t>(v);
				serialize_binary(os, make_object(size));
			}
			catch (boost::numeric::positive_overflow const&) { 
				const std::uint64_t size = std::numeric_limits<std::uint64_t>::max();
				serialize_binary(os, make_object(size));
			}
		}
		
		std::ostream& serializer_custom<SERIALIZE_BINARY>::write(std::ostream& os, std::size_t tag, std::string const& type_name, object const& obj)
		{
			//
			//	type
			//
			//	could throw
			try {
				write_binary(os, boost::numeric_cast<std::uint32_t>(tag));
			}
			catch (boost::numeric::positive_overflow const& ex) {
				std::cerr << std::endl << "*** error: " << ex.what() << std::endl;
				const std::uint32_t tag = traits::PREDEF_CUSTOM;
				write_binary(os, tag);
			}

			//
			//	length
			//
			serialize_length(os, type_name.size());

			//
			//	value
			//
			if (!type_name.empty())
			{
				os.write(type_name.c_str(), type_name.size());
			}

			return os;
		}

	}
}

