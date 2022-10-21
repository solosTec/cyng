/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_PARSE_STRING_H
#define CYNG_PARSE_STRING_H

#include <cyng/obj/buffer_cast.hpp>
#include <cyng/obj/intrinsics/aes_key.hpp>
#include <cyng/obj/util.hpp>
#include <cyng/parse/buffer.h>

#include <chrono>
#include <string>
#include <vector>

#include <boost/asio/ip/address.hpp>
#include <boost/utility/string_view.hpp>
#include <boost/uuid/uuid.hpp>

namespace cyng {
    std::vector<std::string> split(std::string str, std::string delims);
    std::vector<boost::string_view> split(boost::string_view str, boost::string_view delims);

    /**
     * convert hex string to UUID
     */
    [[nodiscard]] boost::uuids::uuid to_uuid(std::string const &);

    [[nodiscard]] boost::uuids::uuid to_uuid(std::string const &, boost::uuids::uuid);

    /**
     * 2021-03-31T19:36:45+0100	- ISO-8601
     */
    [[nodiscard]] std::chrono::system_clock::time_point to_tp_iso8601(std::string const &);

    /**
     * "%Y-%m-%d %H:%M:%S" - SQLite datetime()
     */
    [[nodiscard]] std::chrono::system_clock::time_point to_tp_datetime(std::string const &);

    template <std::size_t N> [[nodiscard]] auto to_aes_key(std::string const &str) -> aes_key<N> {

        BOOST_ASSERT_MSG(str.size() == N / 4, "invalid AES key format");

        auto const buffer = hex_to_buffer(str);
        return make_aes_key<N>(buffer);
    }

    template <std::size_t N> [[nodiscard]] auto to_digest(std::string const &str) -> digest<N> {

        //	valid options for N are 20 (SHA1), 32 (SHA 256) and 64 (SHA 512)
        BOOST_ASSERT_MSG(str.size() == N * 2, "invalid SHA digest format");

        auto const buffer = hex_to_buffer(str);
        return make_digest<N>(buffer);
    }

    namespace {

        template <typename T, int BASE> struct string_policy {
            static T cast(std::string const &str) {
                // static_assert(false, "not a numeric type");
                return T();
            }
        };
        template <int BASE> struct string_policy<std::uint8_t, BASE> {
            static std::uint8_t cast(std::string const &str) {
                try {
                    return static_cast<std::uint8_t>(stoul(str, nullptr, BASE));
                } catch (std::exception const &) {
                }
                return 0;
            }
        };
        template <int BASE> struct string_policy<std::uint16_t, BASE> {
            static std::uint16_t cast(std::string const &str) {
                try {
                    return static_cast<std::uint16_t>(stoul(str, nullptr, BASE));
                } catch (std::exception const &) {
                }
                return 0;
            }
        };
        template <int BASE> struct string_policy<std::uint32_t, BASE> {
            static std::uint32_t cast(std::string const &str) {
                try {
                    return static_cast<std::uint32_t>(stoul(str, nullptr, BASE));
                } catch (std::exception const &) {
                }
                return 0;
            }
        };
        template <int BASE> struct string_policy<std::uint64_t, BASE> {
            static std::uint64_t cast(std::string const &str) {
                try {
                    return static_cast<std::uint64_t>(stoull(str, nullptr, BASE));
                } catch (std::exception const &) {
                }
                return 0;
            }
        };
        template <int BASE> struct string_policy<std::int8_t, BASE> {
            static std::int8_t cast(std::string const &str) {
                try {
                    return static_cast<std::int8_t>(stoi(str, nullptr, BASE));
                } catch (std::exception const &) {
                }
                return 0;
            }
        };
        template <int BASE> struct string_policy<std::int16_t, BASE> {
            static std::int16_t cast(std::string const &str) {
                try {
                    return static_cast<std::int16_t>(stoi(str, nullptr, BASE));
                } catch (std::exception const &) {
                }
                return 0;
            }
        };
        template <int BASE> struct string_policy<std::int32_t, BASE> {
            static std::int32_t cast(std::string const &str) {
                try {
                    return static_cast<std::int32_t>(stol(str, nullptr, BASE));
                } catch (std::exception const &) {
                }
                return 0;
            }
        };
        template <int BASE> struct string_policy<std::int64_t, BASE> {
            static std::int64_t cast(std::string const &str) {
                try {
                    return static_cast<std::int64_t>(stoll(str, nullptr, BASE));
                } catch (std::exception const &) {
                }
                return 0;
            }
        };
        template <int BASE> struct string_policy<float, BASE> {
            static float cast(std::string const &str) {
                try {
                    return stof(str, nullptr);
                } catch (std::exception const &) {
                }
                return 0;
            }
        };
        template <int BASE> struct string_policy<double, BASE> {
            static double cast(std::string const &str) {
                try {
                    return stod(str, 0);
                } catch (std::exception const &) {
                }
                return 0;
            }
        };
        template <int BASE> struct string_policy<long double, BASE> {
            static long double cast(std::string const &str) {
                try {
                    return stold(str, nullptr);
                } catch (std::exception const &) {
                }
                return 0;
            }
        };

    } // namespace

    template <typename T, int BASE = 10> [[nodiscard]] T to_numeric(std::string const &str) {
        return string_policy<T, BASE>::cast(str);
    }

    [[nodiscard]] boost::asio::ip::address to_ip_address(std::string const &);

    template <typename T> [[nodiscard]] boost::asio::ip::basic_endpoint<T> to_ip_endpoint(std::string const &s) {
        auto pos = s.find_last_of(':');
        if (pos != std::string::npos) {
            auto const address = s.substr(0, pos);
            ++pos; //	position after ':'
            if (pos < s.length()) {
                auto const service = s.substr(pos);
                BOOST_ASSERT(!service.empty());
                return boost::asio::ip::basic_endpoint<T>(to_ip_address(address), to_numeric<std::uint16_t>(service));
            }
        }
        return boost::asio::ip::basic_endpoint<T>();
    }

    [[nodiscard]] obis to_obis(std::string const &);

    [[nodiscard]] obis_path_t to_obis_path(std::string const &);

    [[nodiscard]] edis to_edis(std::string const &);

    [[nodiscard]] std::filesystem::path to_fs_path(std::string const &);

    [[nodiscard]] bool is_dec_number(std::string const &);

    [[nodiscard]] bool is_hex_number(std::string const &);
} // namespace cyng
#endif
