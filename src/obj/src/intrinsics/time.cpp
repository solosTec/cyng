#include <cyng/obj/intrinsics/time.h>

#include <algorithm>
#include <iomanip>
#include <locale>
#include <sstream>

#include <time.h>

#include <boost/assert.hpp>

namespace cyng {

    namespace {
        std::tm to_utc(std::time_t tt) {

            std::tm buf{0};
            //
            //  https://stackoverflow.com/questions/69992446/c-location-of-localtime-s-in-gcc
            // Converts a time value to a tm structure.
            // tt is expected to be UTC.
            //
#ifdef _MSC_VER
            decltype(auto) ec = ::gmtime_s(&buf, &tt);
            BOOST_ASSERT(ec == 0); // Zero if successful.
#else
            //	POSIX API
            //            ::gmtime_s(&tt, &buf);
            ::gmtime_r(&tt, &buf);
#endif
            return buf;
        }
        std::tm to_localtime(std::time_t tt) {
            std::tm buf{0};
            // Converts a time_t time value to a tm structure, and corrects for the local time zone.
#ifdef _MSC_VER
            decltype(auto) ec = ::localtime_s(&buf, &tt);
            BOOST_ASSERT(ec == 0); // Zero if successful.
#else
            ::localtime_r(&tt, &buf);
#endif
            return buf;
        }

        /**
         * This non-standard function converts a std::tm struct into
         * a UTC timestamp.
         */
        std::time_t to_utc(std::tm &time) {
            //  Expects that std::tm value is in UTC.
            //  All fields of tm_ are updated to fit their proper ranges
#ifdef _MSC_VER
            return _mkgmtime(&time);
#else
            //  doesn't subtract local timezone
            return timegm(&time);
#endif
        }

    } // namespace

    time::time() noexcept
        : tt_{0} {}

    time::time(std::time_t const &tt) noexcept
        : tt_(tt) {}

    time::time(std::chrono::system_clock::time_point const &tp) noexcept
        : tt_(std::chrono::system_clock::to_time_t(tp)) {}

    time &time::operator=(time const &other) {
        // guard self assignment
        if (this != &other) {
            tt_ = other.tt_;
        }
        return *this;
    }

    time &time::operator=(time &&other) noexcept {
        // guard self assignment
        if (this != &other) {
            tt_ = other.tt_;
        }
        return *this;
    }

    std::tm time::to_utc() const { return cyng::to_utc(tt_); }

    std::tm time::to_localtime() const { return cyng::to_localtime(tt_); }

    time::operator std::chrono::system_clock::time_point() const {
        //  implicit conversion
        return std::chrono::system_clock::from_time_t(tt_);
    }

    std::chrono::seconds time::get_utc_offset(bool dst) const {
        //
        auto utc = to_utc();
        auto diff = time(std::mktime(&utc)) - *this;
        return (dst && utc.tm_isdst > 0) ? diff - std::chrono::seconds(60 * 60) : diff;
    }

    std::chrono::seconds time::to_seconds() const {
        // operator-(time const &tpl, time const &tpr);
        return *this - time();
    }

    time time::get_start_of_day() const noexcept {
        auto const tm = to_localtime();
        std::tm const tmp = {0, 0, 0, tm.tm_mday, tm.tm_mon, tm.tm_year, tm.tm_wday, tm.tm_yday, 0};
        return make_time(tmp);
    }
    time time::get_end_of_day() const noexcept {
        auto const tm = to_localtime();
        std::tm const tmp = {0, 0, 0, tm.tm_mday + 1, tm.tm_mon, tm.tm_year, tm.tm_wday, tm.tm_yday, 0};
        return make_time(tmp);
    }

    time time::get_start_of_month() const noexcept {
        //  doesn't work as expected
        auto const tm = to_localtime();
        std::tm const tmp = {0, 0, 0, 1, tm.tm_mon, tm.tm_year, 0, 0, 0};
        return make_time(tmp);
    }
    time time::get_end_of_month() const noexcept {
        //  doesn't work as expected
        auto const tm = to_localtime();
        //  [0, 11]
        if (tm.tm_mon < 11) {
            //  same year
            std::tm tmp = {0, 0, 0, 1, tm.tm_mon + 1, tm.tm_year, 0, 0, 0};
            return make_time(tmp);
        }
        std::tm tmp = {0, 0, 0, 1, 0, tm.tm_year + 1, 0, 0, 0};
        return make_time(tmp);
    }

    std::chrono::days time::days_in_month() const {
        //  doesn't work as expected
        return std::chrono::duration_cast<std::chrono::days>(get_end_of_month().to_seconds() - get_start_of_month().to_seconds());
    }

    bool operator==(time const &tpl, time const &tpr) { return tpl.tt_ == tpr.tt_; }
    bool operator!=(time const &tpl, time const &tpr) { return !(tpl == tpr); }
    bool operator<(time const &tpl, time const &tpr) { return tpl.tt_ < tpr.tt_; }
    bool operator>(time const &tpl, time const &tpr) { return tpr < tpl; }
    bool operator>=(time const &tpl, time const &tpr) { return !(tpl < tpr); }
    bool operator<=(time const &tpl, time const &tpr) { return !(tpl > tpr); }

    std::chrono::seconds operator-(time const &tpl, time const &tpr) {
        // define an internal duration type to hold seconds represented as double
        using sec = std::chrono::duration<double, std::ratio<1>>;
        sec s(std::difftime(tpl.tt_, tpr.tt_));
        return std::chrono::duration_cast<std::chrono::seconds>(s);
    }

    std::size_t hash(time const &d) {
        std::size_t h{0};
        auto f = std::hash<std::time_t>{};

        //
        //	combine all values
        //
        h ^= f(d.tt_) << 1;
        return h;
    }

    time make_time() { return time(std::time(nullptr)); }
    time make_time(std::tm const &tm) { return time(std::mktime(const_cast<std::tm *>(&tm))); }
    time make_time(int year, int month, int day, int hour, int minute, int second) {
        std::tm const tm{
            second,                           // [0, 60]
            minute,                           // [0, 59]
            hour,                             // [0, 23]
            day,                              // [1, 31]
            month > 0 ? month - 1 : 0,        // [0, 11]
            year > 1900 ? year - 1900 : year, // years since 1900
            0,                                // days since Sunday - [0, 6]
            0,                                // days since January 1 - [0, 365]
            -1                                // try to detect daylight savings time flag
        };
        return make_time(tm);
    }

    time make_time(std::chrono::system_clock::time_point const &tp) {
        //  explicit conversion
        return time(std::chrono::system_clock::to_time_t(tp));
    }

    std::ostream &operator<<(std::ostream &os, time const &t) {
        char str[std::size("yyyy-mm-ddThh:mm:ssZ")];
        std::strftime(std::data(str), std::size(str), "%FT%TZ", std::gmtime(&t.tt_));
        return os << str;
    }

} // namespace cyng

namespace std {

    size_t hash<cyng::time>::operator()(cyng::time const &v) const { return cyng::hash(v); }
} // namespace std
