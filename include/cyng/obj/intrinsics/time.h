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

// see https://gcc.gnu.org/onlinedocs/gcc-9.2.0/libstdc++/api/a00050_source.html
// #if __cplusplus <= 201703L //&& __GNUC__
#if __GNUC__ && (__GNUC__ <= 10)
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
         * This is 00:00 of the this day UTC.
         */
        [[nodiscard]] time get_start_of_day() const noexcept;

        /**
         * This is 00:00 of the next day UTC.
         */
        [[nodiscard]] time get_end_of_day() const noexcept;

        /**
         * This is the first day 00:00 of the this month UTC.
         */
        [[nodiscard]] time get_start_of_month() const noexcept;

        /**
         * This is the first day 00:00 of the next month UTC.
         */
        [[nodiscard]] time get_end_of_month() const noexcept;

        /**
         * @return get_end_of_month() - get_start_of_month()
         */
        [[nodiscard]] std::chrono::days days_in_month() const;

        /**
         * This is the first day 00:00 of the this year UTC.
         */
        [[nodiscard]] time get_start_of_year() const noexcept;

        /**
         * This is the first day 00:00 of the next year UTC.
         */
        [[nodiscard]] time get_end_of_year() const noexcept;

        /**
         * @return get_end_of_year() - get_start_of_year()
         */
        [[nodiscard]] std::chrono::days days_in_year() const noexcept;

        template <typename R, typename P> time add(std::chrono::duration<R, P> d) const {
            //  use implementation of chrono library
            return time(operator std::chrono::system_clock::time_point() + d);
        }

        template <typename R, typename P> time sub(std::chrono::duration<R, P> d) const {
            //  use implementation of chrono library
            return time(operator std::chrono::system_clock::time_point() - d);
        }

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
     * The generated time object is UTC time.
     *
     * @return time object with the time specified in tm.
     */
    time make_time(std::tm const &tm);

    /**
     * The generated time object is UTC time.
     *
     * @return time object with the time as specified.
     */
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

    template <typename R, typename P> time operator+(time const &tp, std::chrono::duration<R, P> d) {
        //  add timespan
        return tp.add<R, P>(d);
    }

    template <typename R, typename P> time operator-(time const &tp, std::chrono::duration<R, P> d) {
        //  sub timespan
        return tp.sub<R, P>(d);
    }

    /**
     * RFC 3339
     */
    std::ostream &operator<<(std::ostream &os, time const &);

    /**
     * @return gregorian year
     */
    constexpr int year(std::tm const &d) { return d.tm_year + 1900; }

    /**
     * @return gregorian month [0, 11]
     */
    constexpr int month(std::tm const &d) { return d.tm_mon + 1; }

    /**
     * @return day of month [1, 31]
     */
    constexpr int day(std::tm const &d) { return d.tm_mday; }

    /**
     * @return day of year
     */
    constexpr int day_of_year(std::tm const &d) { return d.tm_yday; }

    /**
     * @return hour of day
     */
    constexpr int hour(std::tm const &d) { return d.tm_hour; }

    /**
     * @return minute of hour
     */
    constexpr int minute(std::tm const &d) { return d.tm_min; }

    /**
     * @return second of minute
     */
    constexpr int second(std::tm const &d) { return d.tm_sec; }

} // namespace cyng

#include <functional>

namespace std {

    template <> class hash<cyng::time> {
      public:
        size_t operator()(cyng::time const &v) const;
    };
} // namespace std

#endif
