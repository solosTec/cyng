/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_INTRINSCIS_VERSION_H
#define CYNG_OBJ_INTRINSCIS_VERSION_H

#include <boost/predef.h>
#include <cstdint>
#if defined(BOOST_OS_LINUX)
#include <sys/types.h>
#undef major
#undef minor
#endif
// #include <compare>	//	requires gcc 10.x

namespace cyng {

    namespace details {

        template <typename T, typename U> struct part {
            static_assert(sizeof(T) * 2 == sizeof(U), "wrong datatype size");

#pragma pack(push, 1)

            union {
                U full_;
                T t_[2];
            } data_;

#pragma pack(pop)

            constexpr static std::size_t size() noexcept { return sizeof(U); }

            constexpr part(T maj, T min)
                : data_{.t_ = {maj, min}} {}
            constexpr part(U full)
                : data_{.full_ = full} {}

            constexpr T major() const { return data_.t_[0]; }

            constexpr T minor() const { return data_.t_[1]; }

            constexpr U full() const { return data_.full_; }
        };
    } // namespace details

    class version : public details::part<std::uint16_t, std::uint32_t> {
        using base_t = details::part<std::uint16_t, std::uint32_t>;

      public:
        constexpr version(std::uint16_t maj, std::uint16_t min)
            : base_t(maj, min) {}

        constexpr version(std::uint32_t v)
            : base_t(v) {}

        constexpr version(version const &v)
            : base_t(v.major(), v.minor()) {}

        constexpr version()
            : base_t(0, 0) {}
    };

    class revision : public details::part<std::uint32_t, std::uint64_t> {
        using base_t = details::part<std::uint32_t, std::uint64_t>;

      public:
        /**
         * @param a major
         * @param b minor
         * @param c patch
         * @param d tag
         */
        constexpr revision(std::uint16_t a, std::uint16_t b, std::uint16_t c, std::uint16_t d)
            : base_t(version(a, b).full(), version(c, d).full()) {}

        constexpr revision(std::uint32_t maj, std::uint32_t min)
            : base_t(maj, min) {}

        constexpr revision(std::uint64_t v)
            : base_t(v) {}

        constexpr revision(revision const &v)
            : base_t(v.major(), v.minor()) {}

        constexpr revision()
            : base_t(0, 0) {}
    };

    // constexpr std::strong_ordering operator<=>(version lhs, version rhs)
    // {
    // 	return lhs.full() <=> rhs.full();
    // }

    // constexpr std::strong_ordering operator<=>(revision lhs, revision rhs)
    // {
    // 	return lhs.full() <=> rhs.full();
    // }

} // namespace cyng

#include <functional>

namespace std {

    template <> class hash<cyng::version> {
      public:
        size_t operator()(cyng::version const &) const noexcept;
    };
    template <> class hash<cyng::revision> {
      public:
        size_t operator()(cyng::revision const &) const noexcept;
    };

} // namespace std

#endif
