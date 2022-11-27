/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_INTRINSCIS_DATE_H
#define CYNG_OBJ_INTRINSCIS_DATE_H

#include <chrono>
#include <cstdint>
#include <ctime>
#include <string>
#include <ostream>

namespace cyng {

    class date;
    std::size_t hash(date const &d);

    /**
     * Support for type deduction (the old way)
     * Example:
     * @code
     * duration_t<std::chrono::minutes>::_Period p;
     * @endcode
     */
    template <typename T> struct duration_t {
        using type = void;
        using _Rep = void;
        using _Period = void;
    };

    template <typename R, typename P> struct duration_t<std::chrono::duration<R, P>> {
        using type = std::chrono::duration<R, P>;
        using _Rep = R;
        using _Period = P;
    };

    namespace calendar {
        enum month { JANUARY = 1, FEBRUARY, MARCH, APRIL, MAY, JUNE, JULY, AUGUST, SEPTEMBER, OCTOBER, NOVEMBER, DECEMBER };
        enum day { MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY };

        /**
         * Specify the type of timezone
         */
        // class local_tz {};
        // class utc_tz {};
        enum class tz_type { LOCAL, UTC };
    } // namespace calendar

    /**
     * @return a date with the given time since epoch as local time
     */
    date make_date_from_local_time(std::time_t tt);
    date make_date_from_local_time(std::chrono::system_clock::time_point);

    /**
     * This class represents a date with a resolution of 1 second.
     * This class is a small wrapper for the C/C++ time routines that were used before the calendar functions were introduced in
     * C++20. But even with the calendar features available there, it can be tedious to use. The clock class from the cyng_sys
     * library represents such an attempt. Furthermore, the availability of these functions in current compilers is still very
     * limited.
     *
     * The time point is internally stored in a tm struct. Therefore the
     * implementation is not memory efficient. To store a large set of time points use another implementation.
     */
    class date final {
        friend std::size_t hash(date const &);

      public:
        date(calendar::tz_type);
        date(calendar::tz_type, int year, int month, int day, int hour, int minute, int second);
        date(calendar::tz_type, std::tm const &);
        date(date const &) = default;
        date(date &&) = default;

        /**
         * copy assignment
         */
        date &operator=(date const &);
        date &operator=(date);

        /**
         * move assignment
         */
        date &operator=(date &&) noexcept;

        /**
         * @return true if timestamp is utc.
         */
        constexpr bool is_utc() { return tz_type_ == calendar::tz_type::UTC; }

        /**
         * To convert to a UNIX timestamp the function std::mktime() is used.
         * std::mktime() assumes that the internal time stamp is a UTC time and produces
         * a timestamp with local time.
         *
         * @return a UNIX timestamp with local time.
         */
        std::time_t to_local_time() const;
        std::time_t to_utc_time() const;

        explicit constexpr operator std::tm const &() const { return tm_; }

        std::chrono::system_clock::time_point to_time_point() const;

        /**
         * DST is not considered in this calculation.
         *
         * @return difference between UTC and localtime in minutes.
         */
        std::chrono::minutes delta_utc() const;

        [[nodiscard]] date get_start_of_day() const noexcept;
        [[nodiscard]] date get_end_of_day() const noexcept;

        [[nodiscard]] date get_start_of_month() const noexcept;
        [[nodiscard]] date get_end_of_month() const noexcept;
        [[nodiscard]] std::size_t days_in_month() const noexcept;

        [[nodiscard]] date get_start_of_year() const noexcept;
        [[nodiscard]] date get_end_of_year() const noexcept;
        [[nodiscard]] std::size_t days_in_year() const noexcept;

        /**
         * @return a date with the given time since epoch as local time
         */
        // friend date make_date_from_local_time(std::time_t tt);
        // friend date make_date_from_local_time(std::chrono::system_clock::time_point);

        template <typename R, typename P> date add(std::chrono::duration<R, P> d) const {
            //  use implementation of chrono library
            return make_date_from_local_time(to_time_point() + d);
        }

        template <typename R, typename P> date sub(std::chrono::duration<R, P> d) const {
            //  use implementation of chrono library
            return make_date_from_local_time(to_time_point() - d);
        }

        template <typename R, typename P> std::chrono::duration<R, P> sub(date const &other) const {
            //  use implementation of chrono library
            return std::chrono::duration_cast<std::chrono::duration<R, P>>(to_time_point() - other.to_time_point());
        }
        template <typename T> T sub(date const &other) const {
            using R = typename duration_t<T>::_Rep;
            using P = typename duration_t<T>::_Period;
            return sub<R, P>(other);
        }

      private:
        std::tm tm_;
        calendar::tz_type tz_type_;
    };

    /**
     * @return a date with the given time since epoch as UTC
     */
    date make_date_from_utc_time(std::time_t tt);
    date make_date_from_utc_time(std::chrono::system_clock::time_point);

    /**
     * Read a string according to format string and produces a date.
     * Uses the std::get_time() function.
     */
    date make_local_date(std::string const &s, std::string fmt = "%Y-%m-%d %H:%M:%S");
    date make_utc_date(std::string const &s, std::string fmt = "%Y-%m-%d %H:%M:%S");

    /**
     * @return true if object contains a valid time.
     */
    bool is_valid(date const &);

    template <typename R, typename P> date operator+(date const &tp, std::chrono::duration<R, P> d) {
        //  add timespan
        return tp.add<R, P>(d);
    }
    template <typename R, typename P> date operator-(date const &tp, std::chrono::duration<R, P> d) {
        //  sub timespan
        return tp.sub<R, P>(d);
    }

    // template <typename R, typename P> std::chrono::duration<R, P> operator-(date const &tpl, date const &tpr) {
    //     return tpl.sub<R, P>(tpr);
    // }

    /**
     * @return gregorian year
     */
    constexpr int year(date const &d) { return d.operator const tm &().tm_year + 1900; }

    /**
     * @return gregorian month [0, 11]
     */
    constexpr int month(date const &d) { return d.operator const tm &().tm_mon + 1; }

    /**
     * @return day of month [1, 31]
     */
    constexpr int day(date const &d) { return d.operator const tm &().tm_mday; }

    /**
     * @return day of year
     */
    constexpr int day_of_year(date const &d) { return d.operator const tm &().tm_yday; }

    /**
     * @return hour of day
     */
    constexpr int hour(date const &d) { return d.operator const tm &().tm_hour; }

    /**
     * @return minute of hour
     */
    constexpr int minute(date const &d) { return d.operator const tm &().tm_min; }

    /**
     * @return second of minute
     */
    constexpr int second(date const &d) { return d.operator const tm &().tm_sec; }

    void as_string(std::ostream &os, date const &d, std::string format = "%Y-%m-%d %H:%M:%S%z");
    // void as_utc_string(std::ostream &os, date const &d, std::string format = "%Y-%m-%d %H:%M:%S%z");
    std::string as_string(date const &d, std::string format = "%Y-%m-%d %H:%M:%S%z");
    // std::string as_utc_string(date const &d, std::string format = "%Y-%m-%d %H:%M:%S%z");

} // namespace cyng

#include <functional>

namespace std {

    template <> class hash<cyng::date> {
      public:
        size_t operator()(cyng::date const &v) const;
    };
} // namespace std

#endif
