/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_UTIL_HPP
#define CYNG_OBJ_UTIL_HPP

#include <cyng/obj/buffer_cast.hpp>
#include <cyng/obj/factory.hpp>

#include <algorithm>
#include <iterator>

#include <boost/assert.hpp>

#ifdef _DEBUG
#include <iostream>
#endif

namespace cyng {

    /**
     * Create an object from each parameter element and add
     * it to a list of objects (tuple_t)
     *
     * To not be confused with std::make_tuple() add the namespace cyng.
     */
    template <typename... Args> [[nodiscard]] tuple_t make_tuple(Args &&...args) {
        //
        //	Doesn't find the "make_object(const char(&c)[N])" function,
        //	so the internal policy traits must provide the correct function.
        //	It seems that perfect forwarding has it's limits.
        //
        return {make_object<Args>(std::forward<Args>(args))...};
    }

    /**
     * Takes an initializer_list and produce a tuple_t object
     */
    template <typename T> [[nodiscard]] tuple_t make_tuple(std::initializer_list<T> list) {
        tuple_t tpl;
        std::transform(list.begin(), list.end(), std::back_inserter(tpl), [](T const &value) { return cyng::make_object(value); });
        return tpl;
    }

    /**
     * Convert all elements of an initializer list into a vector of objects.
     */
    template <typename T> [[nodiscard]] vector_t make_vector(std::initializer_list<T> list) {
        vector_t vec;
        vec.reserve(list.size());
        std::transform(list.begin(), list.end(), std::back_inserter(vec), [](T const &v) { return make_object(v); });
        return vec;
    }

    /**
     * Create an object from each parameter element and add
     * it to a deque of objects (deque_t)
     */
    template <typename... Args> [[nodiscard]] deque_t make_deque(Args &&...args) {
        return {make_object<Args>(std::forward<Args>(args))...};
    }

    /**
     * Create an attribute
     */
    template <typename... Args> [[nodiscard]] attr_t make_attr(std::size_t idx, Args &&...args) {
        return {idx, make_object(std::forward<Args>(args)...)};
    }

    /**
     * Create a parameter
     */
    template <typename... Args> [[nodiscard]] param_t make_param(std::string const &name, Args &&...args) {
        BOOST_ASSERT_MSG(!name.empty(), "parameter without a name");
        return {name, make_object(std::forward<Args>(args)...)};
    }

    template <typename... Args> [[nodiscard]] param_t make_param(obis const &o, Args &&...args) {
        return {to_string(o), make_object(std::forward<Args>(args)...)};
    }

    template <typename... Args> [[nodiscard]] prop_t make_prop(obis const &o, Args &&...args) {
        return {o, make_object(std::forward<Args>(args)...)};
    }

    template <class CharT, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT>>
    [[nodiscard]] auto make_string(buffer_t const &buffer) -> std::basic_string<CharT, Traits, Allocator> {
        using string_t = std::basic_string<CharT, Traits, Allocator>;
        using value_t = typename string_t::value_type;
        string_t s;
        constexpr static std::size_t size = sizeof(value_t) / sizeof(char);
        s.reserve(buffer.size() / size);
        for (std::size_t idx = 0; idx < buffer.size(); idx += size) {
            auto const r = to_numeric<value_t>(buffer, idx);
            s.push_back(r);
        }
        return s;
    }

    [[nodiscard]] std::string make_string(buffer_t const &, std::size_t offset);

    [[nodiscard]] std::filesystem::path make_fs_path(buffer_t const &);

    [[nodiscard]] mac48 make_mac48(buffer_t const &);

    [[nodiscard]] mac64 make_mac64(buffer_t const &);

    [[nodiscard]] pid make_pid(buffer_t const &);

    [[nodiscard]] obis make_obis(buffer_t const &);
    [[nodiscard]] obis make_obis(buffer_t const &, std::size_t offset);

    [[nodiscard]] obis_path_t make_obis_path(buffer_t const &);

    [[nodiscard]] edis make_edis(buffer_t const &);

    [[nodiscard]] color_8 make_color8(buffer_t const &);
    [[nodiscard]] color_16 make_color16(buffer_t const &);

    template <std::size_t N> [[nodiscard]] auto make_digest(buffer_t const &buffer) -> digest<N> {
        using type = digest<N>;
        if (buffer.size() >= type::size()) {
            typename type::digest_type a{0};
            auto pos = std::begin(buffer);
            for (auto &e : a) {
                BOOST_ASSERT_MSG(pos != std::end(buffer), "buffer to small");
                e = *pos++;
            }
            return type(a);
        }
        return type{};
    }

    template <std::size_t N> auto make_aes_key(buffer_t const &buffer) -> aes_key<N> {
        using type = aes_key<N>;
#ifdef _DEBUG
        // std::cout << buffer.size() << "/" << type::bytes() << std::endl;
#endif
        if (buffer.size() >= type::bytes()) {
            typename type::key_type a{0};
            auto pos = std::begin(buffer);
            for (auto &e : a) {
                BOOST_ASSERT_MSG(pos != std::end(buffer), "buffer to small");
                e = *pos++;
            }
            return type(a);
        }
        return type{};
    }

    [[nodiscard]] boost::uuids::uuid make_uuid(buffer_t const &);

    [[nodiscard]] version make_version(buffer_t const &);

    [[nodiscard]] revision make_revision(buffer_t const &);

    [[nodiscard]] std::chrono::system_clock::time_point make_timepoint(buffer_t const &);

    template <typename T> [[nodiscard]] auto make_duration(buffer_t const &buffer) -> T {

        // std::chrono::duration<R, P>
        using R = typename T::rep;
        auto const rep = to_numeric<R>(buffer);
        return T(rep);
    }

    [[nodiscard]] boost::asio::ip::address make_ip_address(buffer_t const &);

    template <typename T> [[nodiscard]] auto make_endpoint(buffer_t const &buffer) -> boost::asio::ip::basic_endpoint<T> {

        // T => boost::asio::ip::basic_endpoint<T>;
        using ep_t = boost::asio::ip::basic_endpoint<T>;
        auto const port = to_numeric<std::uint16_t>(buffer);
        auto const address = make_string(buffer, sizeof(port));
        //	port + address
        return ep_t(boost::asio::ip::make_address(address), port);
    }

    [[nodiscard]] raw make_raw(buffer_t const &);

    [[nodiscard]] date make_date(buffer_t const &);

    [[nodiscard]] boost::system::error_code make_ec(buffer_t const &);

    [[nodiscard]] op make_op(buffer_t const &buffer);

    [[nodiscard]] severity make_severity(buffer_t const &buffer);

    /**
     * convinience type test
     */
    template <type_code TC> bool is_of_type(object const &obj) { return obj.tag() == TC; }

    /**
     * merge two UUIDs with XOR. This is independend from the
     * ordering of the parameters.
     */
    boost::uuids::uuid merge(boost::uuids::uuid, boost::uuids::uuid);
} // namespace cyng

#endif //	CYNG_OBJ_UTIL_HPP
