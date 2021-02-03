/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <cyng/obj/util.hpp>
#include <cyng/obj/buffer_cast.hpp>

#include <boost/uuid/nil_generator.hpp>
#include <boost/algorithm/string.hpp>

namespace cyng {

	std::string make_string(buffer_t const& buffer) {
		return std::string(std::begin(buffer), std::end(buffer));
	}

	std::string make_string(buffer_t const& buffer, std::size_t offset) {
		return std::string(std::begin(buffer) + offset, std::end(buffer));
	}


	std::filesystem::path make_fs_path(buffer_t const& buffer) {
		return std::filesystem::path(std::begin(buffer), std::end(buffer));
	}

	mac48 make_mac48(buffer_t const& buffer) {
		if (buffer.size() >= mac48::size()) {
			return mac48(
				buffer.at(0),
				buffer.at(1),
				buffer.at(2),
				buffer.at(3),
				buffer.at(4),
				buffer.at(5));
		}
		return mac48{};
	}

	mac64 make_mac64(buffer_t const& buffer) {
		if (buffer.size() >= mac64::size()) {
			return mac64(
				buffer.at(0),
				buffer.at(1),
				buffer.at(2),
				buffer.at(3),
				buffer.at(4),
				buffer.at(5),
				buffer.at(6),
				buffer.at(7));
		}
		return mac64{};
	}

	pid make_pid(buffer_t const& buffer) {
		using value_type = typename pid::value_type;
		return (buffer.size() >= sizeof(value_type)) 
			? pid(to_numeric<value_type>(buffer))
			: pid(0)
			;
	}

	obis make_obis(buffer_t const& buffer) {
		if (buffer.size() >= obis::size()) {
			return obis(
				buffer.at(0),
				buffer.at(1),
				buffer.at(2),
				buffer.at(3),
				buffer.at(4),
				buffer.at(5));
		}
		return obis{};
	}

	boost::uuids::uuid make_uuid(buffer_t const& buffer) {

		boost::uuids::uuid tag = boost::uuids::nil_uuid();
		if (buffer.size() == boost::uuids::uuid::static_size()) {
			std::copy(std::begin(buffer), std::end(buffer), std::begin(tag.data));
		}
		return tag;
	}

	version make_version(buffer_t const& buffer)
	{
		return version(to_numeric<std::uint32_t>(buffer));
	}
	revision make_revision(buffer_t const& buffer)
	{
		return revision(to_numeric<std::uint64_t>(buffer));
	}
	std::chrono::system_clock::time_point make_timepoint(buffer_t const& buffer)
	{
		using rep = std::chrono::system_clock::rep;
		using duration = std::chrono::system_clock::duration;
		return std::chrono::system_clock::time_point(duration(to_numeric<rep>(buffer)));
	}

	boost::asio::ip::address make_ip_address(buffer_t const& buffer)
	{
		return boost::asio::ip::make_address(make_string(buffer));
	}

	boost::system::error_code make_ec(buffer_t const& buffer)
	{
		boost::system::error_code ec;
		auto const code = to_numeric<int>(buffer);
		auto const category = make_string(buffer, sizeof(code));
		if (boost::algorithm::equals(category, "generic")) {
			return boost::system::error_code(code, boost::system::generic_category());
		}
		else if (boost::algorithm::equals(category, "system")) {
			return boost::system::error_code(code, boost::system::system_category());
		}
		return ec;
	}

	op make_op(buffer_t const& buffer) {
		BOOST_ASSERT(buffer.size() == 2);
		return static_cast<op>(to_numeric<std::uint16_t>(buffer));
	}
	severity make_severity(buffer_t const& buffer) {
		BOOST_ASSERT(buffer.size() == 1);
		return static_cast<severity>(to_numeric<std::uint8_t>(buffer));
	}
}

