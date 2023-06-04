/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <cyng/obj/buffer_cast.hpp>
#include <cyng/obj/util.hpp>

#include <iterator>

#include <boost/algorithm/string.hpp>
#include <boost/uuid/nil_generator.hpp>

namespace cyng {

    std::string make_string(buffer_t const &buffer, std::size_t offset) {
        return std::string(std::begin(buffer) + offset, std::end(buffer));
    }

    std::filesystem::path make_fs_path(buffer_t const &buffer) {
        return std::filesystem::path(std::begin(buffer), std::end(buffer));
    }

    mac48 make_mac48(buffer_t const &buffer) {
        if (buffer.size() >= mac48::size()) {
            return mac48(buffer.at(0), buffer.at(1), buffer.at(2), buffer.at(3), buffer.at(4), buffer.at(5));
        }
        return mac48{};
    }

    mac64 make_mac64(buffer_t const &buffer) {
        if (buffer.size() >= mac64::size()) {
            return mac64(
                buffer.at(0), buffer.at(1), buffer.at(2), buffer.at(3), buffer.at(4), buffer.at(5), buffer.at(6), buffer.at(7));
        }
        return mac64{};
    }

    pid make_pid(buffer_t const &buffer) {
        using value_type = typename pid::value_type;
        return (buffer.size() >= sizeof(value_type)) ? pid(to_numeric<value_type>(buffer)) : pid(0);
    }

    obis make_obis(buffer_t const &buffer) { return make_obis(buffer, 0); }

    obis make_obis(buffer_t const &buffer, std::size_t offset) {
        if (buffer.size() + offset >= obis::size()) {
            return obis(
                buffer.at(offset),
                buffer.at(offset + 1),
                buffer.at(offset + 2),
                buffer.at(offset + 3),
                buffer.at(offset + 4),
                buffer.at(offset + 5));
        }
        return obis{};
    }

    obis_path_t make_obis_path(buffer_t const &buffer) {
        BOOST_ASSERT((buffer.size() % obis::size()) == 0);
        auto const size = buffer.size() / obis::size();
        obis_path_t path;
        path.reserve(size);
        for (std::size_t idx = 0; idx < size; ++idx) {
            path.push_back(make_obis(buffer, idx * obis::size()));
        }
        return path;
    }

    edis make_edis(buffer_t const &buffer) {
        if (buffer.size() >= edis::size()) {
            return edis(buffer.at(0), buffer.at(1), buffer.at(2));
        }
        return edis{};
    }

    color_8 make_color8(buffer_t const &buffer) {
        if (buffer.size() >= sizeof(color_8::rgb_type)) {
            return color_8(buffer.at(0), buffer.at(1), buffer.at(2), buffer.at(3));
        }
        return color_8{};
    }
    color_16 make_color16(buffer_t const &buffer) {
        if (buffer.size() >= sizeof(color_16::rgb_type)) {

            auto const r = to_numeric<std::uint16_t>(buffer);
            auto const g = to_numeric<std::uint16_t>(buffer, sizeof(16));
            auto const b = to_numeric<std::uint16_t>(buffer, sizeof(16) * 2);
            auto const o = to_numeric<std::uint16_t>(buffer, sizeof(16) * 3);

            return color_16(r, g, b, o);
        }
        return color_16{};
    }

    boost::uuids::uuid make_uuid(buffer_t const &buffer) {

        boost::uuids::uuid tag = boost::uuids::nil_uuid();
        if (buffer.size() == boost::uuids::uuid::static_size()) {
            std::copy(std::begin(buffer), std::end(buffer), std::begin(tag.data));
        }
        return tag;
    }

    version make_version(buffer_t const &buffer) { return version(to_numeric<std::uint32_t>(buffer)); }
    revision make_revision(buffer_t const &buffer) { return revision(to_numeric<std::uint64_t>(buffer)); }
    std::chrono::system_clock::time_point make_timepoint(buffer_t const &buffer) {
        using rep = std::chrono::system_clock::rep;
        using duration = std::chrono::system_clock::duration;
        return std::chrono::system_clock::time_point(duration(to_numeric<rep>(buffer)));
    }

    boost::asio::ip::address make_ip_address(buffer_t const &buffer) {
        return boost::asio::ip::make_address(make_string<std::string::value_type>(buffer));
    }

    boost::system::error_code make_ec(buffer_t const &buffer) {
        boost::system::error_code ec;
        auto const code = to_numeric<int>(buffer);
        auto const category = make_string(buffer, sizeof(code));
        if (boost::algorithm::equals(category, "generic")) {
            return boost::system::error_code(code, boost::system::generic_category());
        } else if (boost::algorithm::equals(category, "system")) {
            return boost::system::error_code(code, boost::system::system_category());
        }
        return ec;
    }

    raw make_raw(buffer_t const &buffer) {
        auto const code = to_numeric<std::uint16_t>(buffer);
        auto const lit = make_string(buffer, sizeof(code));
        return raw(lit, code);
    }

    date make_date(buffer_t const &buffer) {
        if (buffer.size() >= sizeof(date::unified_date)) {

            auto const a = to_array<std::int32_t, date::SIZE::value>(buffer);
            return date(a);
        }
        return date{};
    }

    time make_time(buffer_t const &buffer) {
        if (buffer.size() >= sizeof(std::uint64_t)) {

            auto const n = to_numeric<std::uint64_t>(buffer);
            return time(n);
        }
        return time{};
    }

    op make_op(buffer_t const &buffer) {
        BOOST_ASSERT(buffer.size() == 2);
        return static_cast<op>(to_numeric<std::uint16_t>(buffer));
    }
    severity make_severity(buffer_t const &buffer) {
        BOOST_ASSERT(buffer.size() == 1);
        return static_cast<severity>(to_numeric<std::uint8_t>(buffer));
    }

    boost::uuids::uuid merge(boost::uuids::uuid u1, boost::uuids::uuid u2) {
        boost::uuids::uuid r;
        for (std::size_t idx = 0; idx < boost::uuids::uuid::static_size(); ++idx) {
            r.data[idx] = u1.data[idx] ^ u2.data[idx];
        }
        return r;
    }

} // namespace cyng
