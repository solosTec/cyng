#include <cyng/obj/intrinsics/date.h>

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
#ifdef _MSC_VER
            decltype(auto) ec = ::localtime_s(&buf, &tt);
            BOOST_ASSERT(ec == 0); // Zero if successful.
#else
            //            ::localtime_s(&tt, &buf);
            ::localtime_r(&tt, &buf);
#endif
            return buf;
        }

        /**
         * This non-standard function converts a std::tm struct into
         * a UTC timestamp.
         */
        std::time_t to_utc(std::tm &time) {
#ifdef _MSC_VER
            return _mkgmtime(&time);
#else
            return timegm(&time);
#endif
        }

    } // namespace

    date::date(calendar::tz_type tzt)
        : date{tzt, 1900, 1, 0, 0, 0, 0} {}

    date::date(calendar::tz_type tzt, int year, int month, int day, int hour, int minute, int second)
        : tm_{second,                           // [0, 60]
              minute,                           // [0, 59]
              hour,                             // [0, 23]
              day,                              // [1, 31]
              month > 0 ? month - 1 : 0,        // [0, 11]
              year > 1900 ? year - 1900 : year, // years since 1900
              0,
              0,
              -1}
        , tz_type_(tzt) {
        //  All fields of tm_ are updated to fit their proper ranges
        if (tz_type_ == calendar::tz_type::LOCAL) {
            std::mktime(&tm_);
        } else {
            to_utc(tm_);
        }
    }

    date::date(calendar::tz_type tzt, std::tm const &tm)
        : tm_(tm)
        , tz_type_(tzt) {}

    date &date::operator=(date const &other) {
        // guard self assignment
        if (this != &other) {
            tm_ = other.tm_;
            tz_type_ = other.tz_type_;
        }
        return *this;
    }

    date &date::operator=(date other) {
        std::swap(tm_, other.tm_);
        std::swap(tz_type_, other.tz_type_);
        return *this;
    }

    date &date::operator=(date &&other) noexcept {
        // guard self assignment
        if (this != &other) {
            tm_ = std::move(other.tm_);
            tz_type_ = other.tz_type_;
        }
        return *this;
    }

    std::time_t date::to_local_time() const {
        auto tmp = tm_; // copy
        //  convert to localtime!
        // tmp.tm_isdst = 0; //  Not daylight saving
        return std::mktime(&tmp);
    }

    std::time_t date::to_utc_time() const {
        auto tmp = tm_; // copy
        return to_utc(tmp);
    }

    // date::operator std::tm const &() const { return tm_; }

    std::chrono::system_clock::time_point date::to_time_point() const {
        auto const tt = to_local_time();
        return std::chrono::system_clock::from_time_t(tt);
    }

    std::chrono::minutes date::delta_utc() const {

        //  localtime
        auto const tt = to_local_time();
        //  utc
        auto const tt_utc = to_utc_time();
        std::chrono::seconds const s(tt - tt_utc);
        return std::chrono::duration_cast<std::chrono::minutes>(s);
    }

    date date::get_start_of_day() const noexcept {
        std::tm const tmp = {0, 0, 0, tm_.tm_wday, tm_.tm_mon, tm_.tm_year, tm_.tm_wday, tm_.tm_yday, tm_.tm_isdst};
        return date(tz_type_, tmp);
    }
    date date::get_end_of_day() const noexcept {
        //    return get_start_of_day().add(std::chrono::hours(24));
        std::tm const tmp = {0, 0, 0, tm_.tm_wday + 1, tm_.tm_mon, tm_.tm_year, tm_.tm_wday, tm_.tm_yday, tm_.tm_isdst};
        return date(tz_type_, tmp);
    }

    date date::get_start_of_month() const noexcept {
        std::tm const tmp = {0, 0, 0, 1, tm_.tm_mon, tm_.tm_year, 0, 0, tm_.tm_isdst};
        return date(tz_type_, tmp);
    }
    date date::get_end_of_month() const noexcept {
        //  [0, 11]
        if (tm_.tm_mon < 11) {
            //  same year
            std::tm tmp = {-1, 0, 0, 1, tm_.tm_mon + 1, tm_.tm_year, 0, 0, -1};

            //  All fields of tm are updated to fit their proper ranges
            std::mktime(&tmp); // localtime!

            return date(tz_type_, tmp);
        }
        std::tm tmp = {-1, 0, 0, 1, 0, tm_.tm_year + 1, 0, 0, -1};
        //  All fields of tm are updated to fit their proper ranges
        std::mktime(&tmp); // localtime!
        return date(tz_type_, tmp);
    }

    std::size_t date::days_in_month() const noexcept {
        //
        auto const h = hours_in_month();
        return h.count() / 24 + 1;
    }

    std::chrono::hours date::hours_in_month() const noexcept {
        auto const s = get_start_of_month();
        return get_end_of_month().sub<std::chrono::hours>(s);
    }

    date date::get_start_of_year() const noexcept {
        std::tm const tmp = {0, 0, 0, 1, 0, tm_.tm_year, 0, 0, -1};
        return date(tz_type_, tmp);
    }
    date date::get_end_of_year() const noexcept {
        //  last second of this year
        std::tm tmp = {-1, 0, 0, 1, 0, tm_.tm_year + 1, 0, 0, -1};
        //  All fields of tm are updated to fit their proper ranges
        std::mktime(&tmp); // localtime!
        return date(tz_type_, tmp);
    }

    std::chrono::hours date::hours_in_year() const noexcept {
        auto const s = get_start_of_year();
        return get_end_of_year().sub<std::chrono::hours>(s);
    }

    std::size_t date::days_in_year() const noexcept {
        auto const h = hours_in_year();
        return h.count() / 24 + 1;
    }

    date make_date_from_local_time(std::time_t tt) {
        auto const tm = to_localtime(tt);
        return date(calendar::tz_type::LOCAL, tm);
    }

    date make_date_from_utc_time(std::time_t tt) {
        auto const tm = to_utc(tt);
        return date(calendar::tz_type::UTC, tm);
    }

    date make_date_from_local_time(std::chrono::system_clock::time_point tp) {
        auto const tt = std::chrono::system_clock::to_time_t(tp);
        return make_date_from_local_time(tt);
    }

    date make_date_from_utc_time(std::chrono::system_clock::time_point tp) {
        auto const tt = std::chrono::system_clock::to_time_t(tp);
        return make_date_from_utc_time(tt);
    }

    date make_local_date(std::string const &s, std::string fmt) {
        std::tm tm = {};
        std::istringstream ss(s);
        ss >> std::get_time(&tm, fmt.c_str());
        return {calendar::tz_type::LOCAL, tm};
    }

    date make_utc_date(std::string const &s, std::string fmt) {
        std::tm tm = {};
        std::istringstream ss(s);
        ss >> std::get_time(&tm, fmt.c_str());
        return {calendar::tz_type::UTC, tm};
    }

    bool is_valid(date const &d) { return d.to_local_time() != -1; }

    void as_string(std::ostream &os, date const &d, std::string format) {
        try {
            //
            //  "%z" timezone works only for local time.
            //
            os << std::put_time(&d.operator const tm &(), format.c_str());
        } catch (std::exception const &ex) {
            os << ex.what();
        }
    }

    //    void as_utc_string(std::ostream &os, date const &d, std::string format) {
    //        try {
    //            auto const offset = std::chrono::duration_cast<std::chrono::seconds>(d.delta_utc());
    // #ifdef _DEBUG
    //            //  debug
    //            std::time_t const ttdebug = d.to_local_time();
    // #endif
    //            //  localtime
    //            std::time_t const tt = d.to_local_time() - offset.count();
    //            auto const tm = to_utc(tt);
    //            os << std::put_time(&tm, format.c_str());
    //        } catch (std::exception const &ex) {
    //            os << ex.what();
    //        }
    //    }

    std::string as_string(date const &d, std::string format) {
        std::stringstream ss;
        as_string(ss, d, format);
        return ss.str();
    }

    // std::string as_utc_string(date const &d, std::string format) {
    //     std::stringstream ss;
    //     as_utc_string(ss, d, format);
    //     return ss.str();
    // }

    std::size_t hash(date const &d) {
        std::size_t h{0};
        auto f = std::hash<int>{};

        //
        //	combine all values
        //
        h ^= f(d.tm_.tm_sec) << 1;
        h ^= f(d.tm_.tm_min) << 1;
        h ^= f(d.tm_.tm_hour) << 1;
        h ^= f(d.tm_.tm_mday) << 1;
        h ^= f(d.tm_.tm_mon) << 1;
        h ^= f(d.tm_.tm_year) << 1;
        return h;
    }

} // namespace cyng

namespace std {

    size_t hash<cyng::date>::operator()(cyng::date const &v) const { return cyng::hash(v); }
} // namespace std
