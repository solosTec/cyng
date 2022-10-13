/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */
#ifndef CYNG_SYS_CLOCK_H
#define CYNG_SYS_CLOCK_H

#include <chrono>
#include <iostream>
#include <string>

namespace cyng {
    namespace sys {
        /**
         * @return the start time of the days relative to tp
         */
        [[nodiscard]] std::chrono::system_clock::time_point get_start_of_day(std::chrono::system_clock::time_point tp) noexcept;
        [[nodiscard]] std::chrono::system_clock::time_point get_end_of_day(std::chrono::system_clock::time_point tp) noexcept;

        [[nodiscard]] std::chrono::system_clock::time_point get_start_of_month(std::chrono::system_clock::time_point) noexcept;
        [[nodiscard]] std::chrono::system_clock::time_point get_end_of_month(std::chrono::system_clock::time_point) noexcept;

        /**
         * @return length of month in hours
         */
        [[nodiscard]] std::chrono::hours get_length_of_month(std::chrono::system_clock::time_point);

        [[nodiscard]] std::chrono::system_clock::time_point get_end_of_year(std::chrono::system_clock::time_point);
        [[nodiscard]] std::chrono::system_clock::time_point get_start_of_year(std::chrono::system_clock::time_point);

        /**
         * @return length of year in hours
         */
        [[nodiscard]] std::chrono::hours get_length_of_year(std::chrono::system_clock::time_point);

        /**
         * Requires the <date/iso_week.h> header
         * @return ISO week number of the specified time point
         */
        //[[nodiscard]] std::uint64_t get_iso_week_number(std::chrono::system_clock::time_point);

        /**
         * Requires the <date/iso_week.h> header
         * @return the day of the week with Monday = 0.
         */
        //[[nodiscard]] std::uint64_t get_day_of_week(std::chrono::system_clock::time_point);

        /**
         * Helper function to format a time point.
         */
        void to_string(std::ostream &os, std::chrono::system_clock::time_point const &tp, std::string format);
        void to_string_utc(std::ostream &os, std::chrono::system_clock::time_point const &tp, std::string format);
        std::string to_string(std::chrono::system_clock::time_point const &tp, std::string format);
        std::string to_string_utc(std::chrono::system_clock::time_point const &tp, std::string format);

        std::tm to_utc(std::time_t);
        std::tm to_localtime(std::time_t);

        /**
         * DST is not considered in this calculation.
         *
         * @return difference between UTC and localtime in minutes.
         */
        std::chrono::minutes delta_utc(std::chrono::system_clock::time_point now);
        std::chrono::minutes delta_utc();

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

    } // namespace sys
} // namespace cyng

#endif
