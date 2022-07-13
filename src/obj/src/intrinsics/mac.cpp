/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <cyng/obj/intrinsics/mac.h>
//#include <cyng/io/ostream.h>
#include <random>
#include <algorithm>
//#include <iostream>

#include <boost/predef.h>
#include <boost/assert.hpp>

namespace cyng
{
	mac48::mac48(boost::uuids::uuid tag)
	: address_({ tag.data[10], tag.data[11], tag.data[12], tag.data[13], tag.data[14], tag.data[15] })
	{}

	mac48& mac48::operator=(mac48 const& other)
	{
		if (this != &other)
		{
			address_ = other.address_;
		}
		return *this;
	}

	bool mac48::is_pause() const
	{
		return *this == mac48(0x01, 0x80, 0xC2, 0x00, 0x00, 0x01);
	}

 	mac48::oui_type mac48::get_oui() const
 	{
		return { std::get<0>(address_)
			, std::get<1>(address_)
			, std::get<2>(address_)
		};
 	}
 	
	mac48::nic_type mac48::get_nic() const
	{
		return { std::get<3>(address_)
			, std::get<4>(address_)
			, std::get<5>(address_)
		};
	}

	boost::asio::ip::address_v6 mac48::to_ipv6_link_local() const
	{
		using value_type = boost::asio::ip::address_v6::bytes_type::value_type;

		//	array< unsigned char, 16 > 
		//	network byte order
		boost::asio::ip::address_v6::bytes_type const bytes {
			0xFE,	//	0
			0x80,	//	1
			0x00,	//	2
			0x00,	//	3
			0x00, 	//	4
			0x00, 	//	5
			0x00, 	//	6
			0x00, 	//	7
			static_cast<value_type>(toogle_kth_bit(address_[0], 2)), 	//	8
			address_[1],	//	9
			address_[2],  	//	10
			0xFF, 	//	11
			0xFE, 	//	12
			address_[3],	//	13
			address_[4], 	//	14
			address_[5] }; 	//	16

		return boost::asio::ip::address_v6(bytes);
	}

	boost::asio::ip::address mac48::to_link_local() const
	{
		return to_ipv6_link_local();
	}

	//	comparison
	bool operator==(mac48 const& lhs, mac48 const& rhs) noexcept
	{
		return lhs.get_octets() == rhs.get_octets();
	}
	bool operator<(mac48 const& lhs, mac48 const& rhs) noexcept
	{
		return lhs.get_octets() < rhs.get_octets();
	}
	bool operator!=(mac48 const& lhs, mac48 const& rhs) noexcept
	{
		return !(lhs == rhs);
	}
	bool operator>(mac48 const& lhs, mac48 const& rhs) noexcept
	{
		//	note the reversed notation
		return rhs < lhs;
	}
	bool operator<=(mac48 const& lhs, mac48 const& rhs) noexcept
	{
		return !(lhs > rhs);
	}
	bool operator>=(mac48 const& lhs, mac48 const& rhs) noexcept
	{
		return !(lhs < rhs);
	}

	buffer_t to_buffer(mac48 const& mac)
	{
		return { mac.get_octets().begin(), mac.get_octets().end() };
	}

	bool is_nil(mac48 const& mac) noexcept	{

		return std::all_of(std::begin(mac.get_octets()), std::end(mac.get_octets()), [](std::uint8_t c){
			return c == 0;
		});
	}

	bool is_broadcast(mac48 const& a)
	{
		return (a == broadcast_address());
	}

	mac48 generate_random_mac48()
	{
#if BOOST_OS_WINDOWS
		std::random_device rnd;
		std::mt19937 gen(rnd());
#else
		std::mt19937 gen;
#endif
		std::uniform_int_distribution<int> dis(std::numeric_limits<std::uint8_t>::min(), std::numeric_limits<std::uint8_t>::max());

		mac48::address_type	addr;
		std::generate(addr.begin(), addr.end(), [&dis, &gen]() {
			return dis(gen);
		});

		//	Mark as private address (second-least significant bit is on)
		addr[0] |= 0x2u;

		return mac48(addr);

	}

	std::size_t hash(mac48 const& mac)
	{
		std::size_t h{ 0 };
		auto f = std::hash<mac48::value_type>{};
		for (auto const& c : mac.get_octets()) {

			//
			//	combine all values
			//
			h ^= f(c) << 1;
		}
		return h;
	}


	//mac64::mac64(std::uint8_t o1, std::uint8_t o2, std::uint8_t o3, std::uint8_t o4, std::uint8_t o5, std::uint8_t o6, std::uint8_t o7, std::uint8_t o8)
	//: address_({ std::uint16_t((o2 << 8) | (o1 & 0xff))
	//	, std::uint16_t((o4 << 8) | (o3 & 0xff))
	//	, std::uint16_t((o6 << 8) | (o5 & 0xff))
	//	, std::uint16_t((o8 << 8) | (o7 & 0xff))
	//})
	//{}
	
	//	use constructor chaining here to save some error prone typing
	mac64::mac64(boost::uuids::uuid tag)
	: mac64( tag.data[8], tag.data[9], tag.data[10], tag.data[11], tag.data[12], tag.data[13], tag.data[14], tag.data[15] )
	{}

	mac64& mac64::operator=(mac64 const& other)
	{
		if (this != &other)
		{
			address_ = other.address_;
		}
		return *this;
	}
	
	mac64::address_type const& mac64::get_words() const
	{
		return address_;
	}


	//	comparison
	bool operator==(mac64 const& lhs, mac64 const& rhs) noexcept
	{
		return lhs.get_words() == rhs.get_words();
	}
	bool operator<(mac64 const& lhs, mac64 const& rhs) noexcept
	{
		return lhs.get_words() < rhs.get_words();
	}
	bool operator!=(mac64 const& lhs, mac64 const& rhs) noexcept
	{
		return !(lhs == rhs);
	}
	bool operator>(mac64 const& lhs, mac64 const& rhs) noexcept
	{
		//	note the reversed notation
		return rhs < lhs;
	}
	bool operator<=(mac64 const& lhs, mac64 const& rhs) noexcept
	{
		return !(lhs > rhs);
	}
	bool operator>=(mac64 const& lhs, mac64 const& rhs) noexcept
	{
		return !(lhs < rhs);
	}

	std::size_t hash(mac64 const& mac)
	{
		std::size_t h{ 0 };
		auto f = std::hash<mac48::value_type>{};
		for (auto const& c : mac.get_words()) {

			//
			//	combine all values
			//
			h ^= f(c) << 1;
		}
		return h;
	}

}

namespace std {

	size_t hash<cyng::mac48>::operator()(cyng::mac48 const& v) const
	{
		return cyng::hash(v);
	}
	size_t hash<cyng::mac64>::operator()(cyng::mac64 const& v) const
	{
		return cyng::hash(v);
	}
}



