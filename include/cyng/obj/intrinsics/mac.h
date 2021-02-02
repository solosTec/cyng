/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_OBJ_INTRINSCIS_MAC_H
#define DOCC_OBJ_INTRINSCIS_MAC_H


#include <cyng/obj/intrinsics/buffer.h>
#include <array>

#include <boost/uuid/uuid.hpp>
#include <boost/asio/ip/address.hpp>

namespace docscript
{
	/** @brief EUI-48
	 *  Media access control address (6 bytes == 48 bits)
	 * @see https://standards.ieee.org/develop/regauth/tut/eui.pdf
	 * @see cyng::mac64
	 * @see cyng::io::mac_parser
	 */
	class mac48
	{
	public:

		using value_type = std::uint8_t;

		using length_t = std::integral_constant<std::size_t, 6>;

		/**
		 * internal storage type
		 */
		using address_type = std::array< value_type, length_t::value >;

		using oui_type = std::array< value_type, 3 >;
		using nic_type = std::array< value_type, 3 >;
		
	public:
		constexpr static std::size_t size() noexcept {
			return length_t::value;
		}

		constexpr mac48()
			: address_()
		{}

		constexpr mac48(std::uint8_t o1, std::uint8_t o2, std::uint8_t o3, std::uint8_t o4, std::uint8_t o5, std::uint8_t o6)
			: address_{ o1, o2, o3, o4, o5, o6 }	//	double-brace required in C+11 but not in C++14
		{}

		constexpr mac48(mac48 const& other)
			: address_(other.address_)
		{}

		constexpr mac48(mac48&& other)  noexcept
			: address_(std::move(other.address_))
		{}

		constexpr mac48(address_type const& address)
			: address_(address)
		{}

		/**
		 * Generate a mac48 address from the last 8 bytes of the 
		 * specified UUID.
		 */
		explicit mac48(boost::uuids::uuid);

		mac48& operator=(mac48 const& other);

		/**
		 * If the least significant bit of the most significant octet of 
		 * an address is set we have a multicast address.
		 * 
		 * @return true if multicast address
		 */
		constexpr bool is_multicast() const {
			//	little endian
			return (std::get<0>(address_) & 0x01) == 0x01;
		}

		
		/**
		 * An address is private if the second-least significant bit 
		 * of the most significant octet of an address is set.
		 * 
		 * Private addresses are:
		 * <ul>
		 * <li>x2-xx-xx-xx-xx-xx</li>
		 * <li>x6-xx-xx-xx-xx-xx</li>
		 * <li>xA-xx-xx-xx-xx-xx</li>
		 * <li>xE-xx-xx-xx-xx-xx</li>
		 * </ul>
		 *
		 * @return true if address is locally administered
		 */
		constexpr bool is_private() const {
			//	little endian
			return (std::get<0>(address_) & 0x02) == 0x02;
		}
		
		/**
		 * @return true if all elements are zero.
		 */
		bool is_nil() const;

		/**
		 * The globally assigned 48-bit multicast address 01-80-C2-00-00-01 has been reserved
		 * for use in MAC Control PAUSE frames for inhibiting transmission of data frames
		 * from a DTE in a full duplex mode IEEE 802.3 LAN. IEEE 802.1D-conformant bridges
		 * will not forward frames sent to this multicast destination address, regardless
		 * of the state of the bridge’s ports, or whether or not the bridge implements the MAC Control sublayer.
		 *
		 * @return true if this is 01:80:C2:00:00:01
		 */
		bool is_pause() const;

		/**
		 * @return array with all octets.
		 */
		address_type const& get_octets() const;

		/**
		 * @return network interface controller (NIC)
		 */
		nic_type get_nic() const;

		/**
		 * @return organisation unique identifier (OUI)
		 */
		oui_type get_oui() const;
		
		/**
		 * generate the link-local address (IPv6)
		 */
		boost::asio::ip::address_v6 to_ipv6_link_local() const;
		boost::asio::ip::address to_link_local() const;

	private:
		address_type	address_;
	};
	
	//	comparison
	bool operator==(mac48 const&, mac48 const&) noexcept;
	bool operator<(mac48 const&, mac48 const&) noexcept;
	bool operator!=(mac48 const&, mac48 const&) noexcept;
	bool operator>(mac48 const&, mac48 const&) noexcept;
	bool operator<=(mac48 const&, mac48 const&) noexcept;
	bool operator>=(mac48 const&, mac48 const&) noexcept;

	/**
	 * make a copy
	 */
	buffer_t to_buffer(mac48 const& mac);

	/**
	 * calculate hash
	 */
	std::size_t hash(mac48 const& address);

	/**
	 * @return a broadcast address FF:FF:FF:FF:FF:FF
	 */
	constexpr mac48 broadcast_address() {
		return mac48(0xff, 0xff, 0xff, 0xff, 0xff, 0xff);
	}

	/**
	 * @return true if specified address is a broadcast address
	 */
	bool is_broadcast(mac48 const&);

	/**
	 * Generate a random private MAC
	 */
	mac48 generate_random_mac48();

	/** @brief EUI-64
	 * Media access control address (8 bytes == 64 bits)
	 * @see https://standards.ieee.org/develop/regauth/tut/eui.pdf
	 * @see cyng::mac48
	 */
	class mac64
	{
	public:
		using value_type = std::uint16_t;

		using length_t = std::integral_constant<std::size_t, 4>;

		/**
		 * internal storage type
		 */
		using address_type = std::array< value_type, length_t::value>;
		
	public:
		constexpr static std::size_t size() noexcept {
			return length_t::value * 2;
		}

		constexpr mac64()
			: address_()
		{}

		constexpr mac64(std::uint8_t o1, std::uint8_t o2, std::uint8_t o3, std::uint8_t o4, std::uint8_t o5, std::uint8_t o6, std::uint8_t o7, std::uint8_t o8)
			: address_({ std::uint16_t((o2 << 8) | (o1 & 0xff))
				, std::uint16_t((o4 << 8) | (o3 & 0xff))
				, std::uint16_t((o6 << 8) | (o5 & 0xff))
				, std::uint16_t((o8 << 8) | (o7 & 0xff))
			})
		{}

		constexpr mac64(std::uint16_t a, std::uint16_t b, std::uint16_t c, std::uint16_t d)
			: address_{ a, b, c, d }	//	double-brace required in C+11 but not in C++14
		{}

		constexpr mac64(mac64 const& other)
			: address_(other.address_)
		{}

		constexpr mac64(mac64&& other) noexcept
			: address_(std::move(other.address_))
		{}

		constexpr mac64(address_type const& address)
			: address_(address)
		{}

		/**
		* Generate a mac48 address from the last 6 bytes of the
		* specified UUID.
		*/
		mac64(boost::uuids::uuid);

		mac64& operator=(mac64 const& other);
		
		address_type const& get_words() const;

	private:
		address_type	address_;
	};

	bool operator==(mac64 const&, mac64 const&) noexcept;
	bool operator<(mac64 const&, mac64 const&) noexcept;
	bool operator!=(mac64 const&, mac64 const&) noexcept;
	bool operator>(mac64 const&, mac64 const&) noexcept;
	bool operator<=(mac64 const&, mac64 const&) noexcept;
	bool operator>=(mac64 const&, mac64 const&) noexcept;

	/**
	 * calculate hash
	 */
	std::size_t hash(mac64 const& address);

	constexpr int toogle_kth_bit(unsigned int n, int k) {
		return (n ^ (1 << (k - 1)));
	}
}

#include <functional>

namespace std {

	template <>
	class hash<docscript::mac48> {
	public:
		size_t operator()(docscript::mac48 const& v) const;
	};

	template <>
	class hash<docscript::mac64> {
	public:
		size_t operator()(docscript::mac64 const& v) const;
	};
}

#endif 	// CYNG_INTRINSICS_MAC_H

