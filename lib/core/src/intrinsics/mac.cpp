/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/intrinsics/mac.h>
#include <cyng/util/slice.hpp>
#include <random>
#include <algorithm>

#include <boost/predef.h>
#include <boost/assert.hpp>

namespace cyng 
{
	mac48::mac48()
	: address_()
	{}
	
	mac48::mac48(std::uint8_t o1, std::uint8_t o2, std::uint8_t o3, std::uint8_t o4, std::uint8_t o5, std::uint8_t o6)
	: address_({o1, o2, o3, o4, o5, o6})	//	double-brace required in C+11 but not in C++14
	{}
	
	mac48::mac48(mac48 const& other)
	: address_(other.address_)
	{}

	mac48::mac48(mac48&& other)
	: address_(std::move(other.address_))
	{}

	mac48::mac48(address_type const& address)
	: address_(address)
	{}

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

	bool mac48::is_multicast() const
	{
		//	little endian
		return (address_[ 0 ] & 0x01) == 0x01;
	}

	bool mac48::is_private() const
	{
		//	little endian
		return (address_[ 0 ] & 0x02) == 0x02;
	}

	bool mac48::is_nil() const
	{
		for (const auto c : address_)
		{
			if (c != 0U)
			{
				return false;
			}
		}
		return true;	
	}

	bool mac48::is_pause() const
	{
		return *this == mac48(0x01, 0x80, 0xC2, 0x00, 0x00, 0x01);
	}

	mac48::address_type const& mac48::get_octets() const
	{
		return address_;
	}
		
	buffer_t mac48::to_buffer() const
	{
		return buffer_t(address_.begin(), address_.end());
	}

 	mac48::oui_type const& mac48::get_oui() const
 	{
		return slice<0, 3>(address_);
 	}
 	
	mac48::nic_type const& mac48::get_nic() const
	{
		return slice<3, 6>(address_);
	}

	//static 
	mac48 mac48::get_broadcast_address()
	{
		return mac48(0xff, 0xff, 0xff, 0xff, 0xff, 0xff);
	}

	boost::asio::ip::address_v6 mac48::to_ipv6_link_local() const
	{
		//	array< unsigned char, 16 > 
		//	network byte order
		boost::asio::ip::address_v6::bytes_type const bytes{
			0xFE,	//	0
			0x80,	//	1
			0x00,	//	2
			0x00,	//	3
			0x00, 	//	4
			0x00, 	//	5
			0x00, 	//	6
			0x00, 	//	7
			toogle_kth_bit(address_[0], 2), 	//	8
			address_[1],	//	9
			address_[2],  	//	10
			0xFF, 	//	11
			0xFE, 	//	12
			address_[3],	//	13
			address_[4], 	//	14
			address_[5] }; 	//	16

		return boost::asio::ip::address_v6(bytes);
	}

	//	comparison
	bool operator==(mac48 const& lhs, mac48 const& rhs)
	{
		return lhs.get_octets() == rhs.get_octets();
	}
	bool operator<(mac48 const& lhs, mac48 const& rhs)
	{
		return lhs.get_octets() < rhs.get_octets();
	}
	bool operator!=(mac48 const& lhs, mac48 const& rhs)
	{
		return !(lhs == rhs);
	}
	bool operator>(mac48 const& lhs, mac48 const& rhs)
	{
		//	note the reversed notation
		return rhs < lhs;
	}
	bool operator<=(mac48 const& lhs, mac48 const& rhs)
	{
		return !(lhs > rhs);
	}
	bool operator>=(mac48 const& lhs, mac48 const& rhs)
	{
		return !(lhs < rhs);
	}

	bool is_broadcast(mac48 const& a)
	{
		return (a == mac48::get_broadcast_address());
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


	mac64::mac64()
	: address_()
	{}
	
	mac64::mac64(std::uint8_t o1, std::uint8_t o2, std::uint8_t o3, std::uint8_t o4, std::uint8_t o5, std::uint8_t o6, std::uint8_t o7, std::uint8_t o8)
	: address_({ std::uint16_t((o2 << 8) | (o1 & 0xff))
		, std::uint16_t((o4 << 8) | (o3 & 0xff))
		, std::uint16_t((o6 << 8) | (o5 & 0xff))
		, std::uint16_t((o8 << 8) | (o7 & 0xff))
	})
	{}
	
	mac64::mac64(std::uint16_t a, std::uint16_t b, std::uint16_t c, std::uint16_t d)
	: address_({ a, b, c, d })	//	double-brace required in C+11 but not in C++14
	{}

	mac64::mac64(mac64 const& other)
	: address_(other.address_)
	{}

	mac64::mac64(mac64&& other)
	: address_(std::move(other.address_))
	{}

	mac64::mac64(address_type const& address)
	: address_(address)
	{}

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
	bool operator==(mac64 const& lhs, mac64 const& rhs)
	{
		return lhs.get_words() == rhs.get_words();
	}
	bool operator<(mac64 const& lhs, mac64 const& rhs)
	{
		return lhs.get_words() < rhs.get_words();
	}
	bool operator!=(mac64 const& lhs, mac64 const& rhs)
	{
		return !(lhs == rhs);
	}
	bool operator>(mac64 const& lhs, mac64 const& rhs)
	{
		//	note the reversed notation
		return rhs < lhs;
	}
	bool operator<=(mac64 const& lhs, mac64 const& rhs)
	{
		return !(lhs > rhs);
	}
	bool operator>=(mac64 const& lhs, mac64 const& rhs)
	{
		return !(lhs < rhs);
	}

	int toogle_kth_bit(unsigned int n, int k) {
		return (n ^ (1 << (k - 1)));
	}

}




