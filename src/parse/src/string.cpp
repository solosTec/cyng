/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#include <cyng/parse/string.h>

#include <iomanip>

#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/assert.hpp>
#include <boost/uuid/nil_generator.hpp>
#include <boost/uuid/string_generator.hpp>

namespace cyng {

    std::vector<std::string> split(std::string str, std::string delims) {
        std::vector<std::string> result;
        boost::algorithm::split(result, str, boost::algorithm::is_any_of(delims), boost::algorithm::token_compress_on);
        return result;
    }

    std::vector<boost::string_view> split(boost::string_view str, boost::string_view delims) {

        std::vector<boost::string_view> result;

        boost::string_view::size_type start = 0;
        auto pos = str.find_first_of(delims, start);
        while (pos != boost::string_view::npos) {
            if (pos != start) {
                result.push_back(str.substr(start, pos - start));
            }
            start = pos + 1;
            pos = str.find_first_of(delims, start);
        }

        if (start < str.length()) {
            result.push_back(str.substr(start, str.length() - start));
        }
        return result;
    }

    boost::uuids::uuid to_uuid(std::string const &str) { return to_uuid(str, boost::uuids::nil_uuid()); }

    boost::uuids::uuid to_uuid(std::string const &str, boost::uuids::uuid tag) {

        try {
            return boost::uuids::string_generator()(str);
        } catch (std::exception const &) {
        }
        return tag;
    }

    //	2021-03-31T19:36:45+0100	- ISO-8601
    std::chrono::system_clock::time_point to_tp_iso8601(std::string const &str) {

        std::tm tm = {};
        std::stringstream ss(str);
        ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S%z");
        return std::chrono::system_clock::from_time_t(std::mktime(&tm));
    }

    //	"%Y-%m-%d %H:%M:%S" - SQLite datetime()
    std::chrono::system_clock::time_point to_tp_datetime(std::string const &str) {

        std::tm tm = {};
        std::stringstream ss(str);
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        return std::chrono::system_clock::from_time_t(std::mktime(&tm));
    }

    boost::asio::ip::address to_ip_address(std::string const &str) {

        boost::system::error_code ec;
        return boost::asio::ip::make_address(str, ec);
    }

    obis to_obis(std::string const &str) {
        //	six hex numbers
        auto const buffer = hex_to_buffer(str);
        if (buffer.size() == obis::size()) {
            return {
                static_cast<std::uint8_t>(buffer.at(0)),
                static_cast<std::uint8_t>(buffer.at(1)),
                static_cast<std::uint8_t>(buffer.at(2)),
                static_cast<std::uint8_t>(buffer.at(3)),
                static_cast<std::uint8_t>(buffer.at(4)),
                static_cast<std::uint8_t>(buffer.at(5))};
        }

        return obis();
    }

    obis_path_t to_obis_path(std::string const &str) {
        //	"obis:obis:obis"
        obis_path_t r;
        auto const v = split(str, ":");
        std::transform(std::begin(v), std::end(v), std::inserter(r, r.end()), [](std::string const &s) { return to_obis(s); });
        return r;
    }

    edis to_edis(std::string const &str) {
        //	something like "c.d.e" with c, d, e as decimal numbers
        std::array<std::uint8_t, edis::size()> a = {0};
        auto const v = split(str, ".");
        if (v.size() == edis::size()) {
            std::transform(
                std::begin(v), std::begin(v), a.begin(), [](std::string const &s) { return to_numeric<std::uint8_t, 10>(s); });
        }
        return {a.at(0), a.at(1), a.at(2)};
    }

    std::filesystem::path to_fs_path(std::string const &s) {
        //	"path"
        std::string str = s;
        str.erase(remove(str.begin(), str.end(), '\"'), str.end());
        try {
            return std::filesystem::path(str);
        } catch (...) {
        }
        return std::filesystem::path();
    }

    bool is_dec_number(std::string const &s) {
        return std::all_of(s.begin(), s.end(), [](std::string::value_type c) -> bool {
            switch (c) {
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0': return true;
            default: break;
            }
            return false;
        });
    }

    bool is_hex_number(std::string const &s) {
        return std::all_of(s.begin(), s.end(), [](std::string::value_type c) -> bool {
            switch (c) {
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F': return true;
            default: break;
            }
            return false;
        });
    }

} // namespace cyng
