/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_INTRINSICS_MAC_H
#define CYNG_INTRINSICS_MAC_H


#include <cyng/intrinsics/buffer.h>
#include <array>
#include <boost/uuid/uuid.hpp>

namespace cyng 
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

		/**
		 * internal storage type
		 */
		using address_type = std::array< value_type, 6 >;

		using oui_type = std::array< value_type, 3 >;
		using nic_type = std::array< value_type, 3 >;
		
	public:
		mac48();
		mac48(std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t);
		mac48(mac48 const& other);
		mac48(mac48&& other);
		mac48(address_type const& other);

		/**
		 * Generate a mac48 address from the last 8 bytes of the 
		 * specified UUID.
		 */
		mac48(boost::uuids::uuid);

		mac48& operator=(mac48 const& other);

		/**
		 * If the least significant bit of the most significant octet of 
		 * an address is set we have a multicast address.
		 * 
		 * @return true if multicast address
		 */
		bool is_multicast() const;
		
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
		bool is_private() const;		
		
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
		buffer_t to_buffer() const;

		/**
		 * @return network interface controller (NIC)
		 */
		nic_type const& get_nic() const;

		/**
		 * @return organisation unique identifier (OUI)
		 */
		oui_type const& get_oui() const;
		
		/**
		 * @return a broadcast address FF:FF:FF:FF:FF:FF
		 */
		static mac48 get_broadcast_address();

	private:
		address_type	address_;
	};
	
	//	comparison
	bool operator==(mac48 const&, mac48 const&);
	bool operator<(mac48 const&, mac48 const&);
	bool operator!=(mac48 const&, mac48 const&);
	bool operator>(mac48 const&, mac48 const&);
	bool operator<=(mac48 const&, mac48 const&);
	bool operator>=(mac48 const&, mac48 const&);

	
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

		/**
		 * internal storage type
		 */
		using address_type = std::array< value_type, 4 >;
		
	public:
		mac64();
		mac64(std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t);
		mac64(std::uint16_t, std::uint16_t, std::uint16_t, std::uint16_t);
		mac64(mac64 const& other);
		mac64(mac64&&);
		mac64(address_type const& other);

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

	bool operator==(mac64 const&, mac64 const&);
	bool operator<(mac64 const&, mac64 const&);
	bool operator!=(mac64 const&, mac64 const&);
	bool operator>(mac64 const&, mac64 const&);
	bool operator<=(mac64 const&, mac64 const&);
	bool operator>=(mac64 const&, mac64 const&);


}

#endif 	// CYNG_INTRINSICS_MAC_H

