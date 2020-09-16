/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_IO_SERIALIZE_TYPED_HPP
#define CYNG_IO_SERIALIZE_TYPED_HPP

#include <cyng/io/serializer/plain.hpp>
#include <cyng/intrinsics/traits/tag_names.hpp>

namespace cyng 
{	
	namespace io
	{	
		template <>
		struct serializer <bool, SERIALIZE_TYPED>
		{
			static std::ostream& write(std::ostream& os, bool v);
		};

		template <>
		struct serializer <version, SERIALIZE_TYPED>
		{
			static std::ostream& write(std::ostream& os, version const& v);
		};
		
		template <>
		struct serializer <revision, SERIALIZE_TYPED>
		{
			static std::ostream& write(std::ostream& os, revision const& v);
		};

		template <>
		struct serializer <std::chrono::system_clock::time_point, SERIALIZE_TYPED>
		{
			static std::ostream& write(std::ostream& os, std::chrono::system_clock::time_point tp);
		};
 		
		template <typename R, typename P>
		struct serializer <std::chrono::duration<R, P>, SERIALIZE_TYPED>
		{
			using type = typename std::chrono::duration<R, P>;
			static std::ostream& write(std::ostream& os, std::chrono::duration<R, P> const& v)
			{
				serializer<type, SERIALIZE_PLAIN>::write(os, v);
				os << ':' << cyng::traits::get_tag_name<type>();
				return os;
			}
		};

		template <>
 		struct serializer <std::string, SERIALIZE_TYPED>
 		{
 			static std::ostream& write(std::ostream& os, std::string const& v);
 		};
 		
 		template <>
 		struct serializer <boost::uuids::uuid, SERIALIZE_TYPED>
 		{
 			static std::ostream& write(std::ostream& os, boost::uuids::uuid const& v);
 		};

 		template <>
 		struct serializer <tuple_t, SERIALIZE_TYPED>
 		{
 			static std::ostream& write(std::ostream& os, tuple_t const& v);
 		};
 
 		template <>
 		struct serializer <vector_t, SERIALIZE_TYPED>
 		{
 			static std::ostream& write(std::ostream& os, vector_t const& v);
 		};
 
 		template <>
 		struct serializer <set_t, SERIALIZE_TYPED>
 		{
 			static std::ostream& write(std::ostream& os, set_t const& v);
 		};

		template <>
		struct serializer <attr_map_t, SERIALIZE_TYPED>
		{
			static std::ostream& write(std::ostream& os, attr_map_t const& v);
		};

		template <>
		struct serializer <param_map_t, SERIALIZE_TYPED>
		{
			static std::ostream& write(std::ostream& os, param_map_t const& v);
		};

		// 	template <>
	// 	struct serializer <boost::filesystem::path, SERIALIZE_TYPED>
	// 	{
	// 		static std::ostream& write(std::ostream& os, boost::filesystem::path const& v);
	// 	};
		
		template <>
		struct serializer <color_8, SERIALIZE_TYPED>
		{
			static std::ostream& write(std::ostream& os, color_8 const& v);
		};

		template <>
		struct serializer <color_16, SERIALIZE_TYPED>
		{
			static std::ostream& write(std::ostream& os, color_16 const& v);
		};

		template <>
		struct serializer <std::int8_t, SERIALIZE_TYPED>
		{
			static std::ostream& write(std::ostream& os, std::int8_t v);
		};

		template <>
		struct serializer <std::uint8_t, SERIALIZE_TYPED>
		{
			static std::ostream& write(std::ostream& os, std::uint8_t v);
		};

		template <>
		struct serializer <std::int16_t, SERIALIZE_TYPED>
		{
			static std::ostream& write(std::ostream& os, std::uint16_t v);
		};

		template <>
		struct serializer <std::uint16_t, SERIALIZE_TYPED>
		{
			static std::ostream& write(std::ostream& os, std::uint16_t v);
		};

		template <>
		struct serializer <std::int32_t, SERIALIZE_TYPED>
		{
			static std::ostream& write(std::ostream& os, std::int32_t v);
		};

		template <>
		struct serializer <std::uint32_t, SERIALIZE_TYPED>
		{
			static std::ostream& write(std::ostream& os, std::uint32_t v);
		};

		template <>
		struct serializer <std::int64_t, SERIALIZE_TYPED>
		{
			static std::ostream& write(std::ostream& os, std::int64_t v);
		};

		template <>
		struct serializer <std::uint64_t, SERIALIZE_TYPED>
		{
			static std::ostream& write(std::ostream& os, std::uint64_t v);
		};

		template <>
		struct serializer <buffer_t, SERIALIZE_TYPED>
		{
			static std::ostream& write(std::ostream& os, buffer_t const& v);
		};

		template <>
		struct serializer <boost::asio::ip::tcp::endpoint, SERIALIZE_TYPED>
		{
			static std::ostream& write(std::ostream& os, boost::asio::ip::tcp::endpoint const&);
		};

		template <>
		struct serializer <boost::asio::ip::udp::endpoint, SERIALIZE_TYPED>
		{
			static std::ostream& write(std::ostream& os, boost::asio::ip::udp::endpoint const&);
		};

		template <>
		struct serializer <boost::asio::ip::icmp::endpoint, SERIALIZE_TYPED>
		{
			static std::ostream& write(std::ostream& os, boost::asio::ip::icmp::endpoint const&);
		};

		template <>
		struct serializer <boost::asio::ip::address, SERIALIZE_TYPED>
		{
			static std::ostream& write(std::ostream& os, boost::asio::ip::address const&);
		};

	}
}

#endif // CYNG_IO_SERIALIZE_TYPED_HPP

