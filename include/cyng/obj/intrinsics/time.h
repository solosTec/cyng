/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_INTRINSCIS_TIME_H
#define CYNG_OBJ_INTRINSCIS_TIME_H

#include <array>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <ostream>
#include <string>

#include <boost/assert.hpp>

#if __cplusplus <= 201703L && __GNUC__
namespace std {
    namespace chrono {
        using days = duration<int, ratio_multiply<ratio<24>, hours::period>>;
        using weeks = duration<int, ratio_multiply<ratio<7>, days::period>>;
        using years = duration<int, ratio_multiply<ratio<146097, 400>, days::period>>;
        using months = duration<int, ratio_divide<years::period, ratio<12>>>;
    } // namespace chrono
} // namespace std
#endif // _HAS_CXX20

namespace cyng {

    /**
     * This class represents a date with a resolution of 1 second.
     * This class is a simple wrapper for std::time_t.
     */
    class time final {
        friend std::size_t hash(time const &);
        friend bool operator==(time const &, time const &);
        friend bool operator<(time const &, time const &);
        friend std::chrono::seconds operator-(time const &, time const &);
        friend std::ostream &operator<<(std::ostream &os, time const &);

      public:
        time() noexcept;
        time(time const &) noexcept = default;
        time(time &&) noexcept = default;
        explicit time(std::time_t const &tt) noexcept;
        explicit time(std::chrono::system_clock::time_point const &tp) noexcept;

        /**
         * copy assignment
         */
        time &operator=(time const &);

        /**
         * move assignment
         */
        time &operator=(time &&) noexcept;

        /**
         * @return time as UTC
         */
        std::tm to_utc() const;

        /**
         * @return time as local time
         */
        std::tm to_localtime() const;

        /**
         * @return time as time_point of system clock
         */
        explicit operator std::chrono::system_clock::time_point() const;

        /**
         * @param dst take daylight saving time into account
         * @return The difference to UTC in seconds
         */
        std::chrono::seconds get_utc_offset(bool dst) const;

        /**
         * @return seconds since start of epoch
         */
        std::chrono::seconds to_seconds() const;

        /**
         * This is 00:00 of the this day.
         */
        [[nodiscard]] time get_start_of_day() const noexcept;

        /**
         * This is 00:00 of the next day.
         */
        [[nodiscard]] time get_end_of_day() const noexcept;

        [[nodiscard]] time get_start_of_month() const noexcept;

        /**
         * This is the first day 00:00 of the next month.
         */
        [[nodiscard]] time get_end_of_month() const noexcept;

        [[nodiscard]] std::chrono::days days_in_month() const;

      private:
        /**
         * Although not defined, this is almost always an integral value holding the number
         * of seconds (not counting leap seconds) since 00:00, Jan 1 1970 UTC.
         */
        std::time_t tt_;
    };

    /**
     * calculate a hash value
     */
    std::size_t hash(time const &d);

    /**
     * @return time object with current time
     */
    time make_time();

    /**
     * @return time object with the time specified in tm.
     */
    time make_time(std::tm const &tm);
    time make_time(int year, int month, int day, int hour, int minute, int second);

    /**
     * Wrapper for std::chrono::system_clock::from_time_t()
     *
     * @return time object
     */
    time make_time(std::chrono::system_clock::time_point const &tm);

    /**
     * Comparison operators
     */
    bool operator==(time const &tpl, time const &tpr);
    bool operator!=(time const &tpl, time const &tpr);
    bool operator<(time const &tpl, time const &tpr);
    bool operator>(time const &tpl, time const &tpr);
    bool operator>=(time const &tpl, time const &tpr);
    bool operator<=(time const &tpl, time const &tpr);

    /**
     * Wrapper for std::difftime
     */
    std::chrono::seconds operator-(time const &tpl, time const &tpr);

    /**
     * RFC 3339
     */
    std::ostream &operator<<(std::ostream &os, time const &);

} // namespace cyng

#include <functional>

namespace std {

    template <> class hash<cyng::time> {
      public:
        size_t operator()(cyng::time const &v) const;
    };
} // namespace std

#endif
