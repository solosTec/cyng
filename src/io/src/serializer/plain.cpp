/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#include <cyng/io/ostream.h>
#include <cyng/io/parser/utf-8.hpp>
#include <cyng/io/serializer/plain.hpp>

#include <boost/io/ios_state.hpp>

#include <algorithm>
#include <iomanip>

namespace cyng {
    namespace io {

        std::size_t serializer<bool, PLAIN>::write(std::ostream &os, bool v) {
            boost::io::ios_flags_saver ifs(os);
            calc_size const cs(os);
            os << std::boolalpha << v;
            return cs;
        }

        std::size_t serializer<boost::uuids::uuid, PLAIN>::write(std::ostream &os, boost::uuids::uuid const &v) {
            //	store and reset stream state
            boost::io::ios_flags_saver ifs(os);
            calc_size const cs(os);

            os << std::hex << std::setfill('0');

            //
            // 	format xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
            //	36 characters

            std::size_t idx{0};
            for (auto const c : v) {
                os << std::setw(2) << (+c & 0xFF) //	promote to integer
                    ;

                ++idx;
                if (idx == 4 || idx == 6 || idx == 8 || idx == 10) {
                    os << '-';
                }
            }
            return cs;
        }

        std::size_t serializer<std::int8_t, PLAIN>::write(std::ostream &os, std::int8_t v) {
            //	store and reset stream state
            boost::io::ios_flags_saver ifs(os);
            calc_size const cs(os);

            os << std::dec << std::setfill('0') << +v;
            return cs;
        }

        std::size_t serializer<std::int16_t, PLAIN>::write(std::ostream &os, std::int16_t v) {
            //	store and reset stream state
            boost::io::ios_flags_saver ifs(os);
            calc_size const cs(os);

            os << std::dec << v;
            return cs;
        }

        std::size_t serializer<std::int32_t, PLAIN>::write(std::ostream &os, std::int32_t v) {
            //	store and reset stream state
            boost::io::ios_flags_saver ifs(os);
            calc_size const cs(os);

            os << std::dec << v;
            return cs;
        }

        std::size_t serializer<std::int64_t, PLAIN>::write(std::ostream &os, std::int64_t v) {
            //	store and reset stream state
            boost::io::ios_flags_saver ifs(os);
            calc_size const cs(os);

            os << std::dec << v;
            return cs;
        }

        std::size_t serializer<std::uint8_t, PLAIN>::write(std::ostream &os, std::uint8_t v) {
            //	store and reset stream state
            boost::io::ios_flags_saver ifs(os);
            calc_size const cs(os);

            os << std::hex << std::setfill('0') << +v;
            return cs;
        }
        std::size_t serializer<std::uint16_t, PLAIN>::write(std::ostream &os, std::uint16_t v) {
            //	store and reset stream state
            boost::io::ios_flags_saver ifs(os);
            calc_size const cs(os);

            os << std::hex << std::setfill('0') << std::setw(4) << v;
            return cs;
        }
        std::size_t serializer<std::uint32_t, PLAIN>::write(std::ostream &os, std::uint32_t v) {
            //	store and reset stream state
            boost::io::ios_flags_saver ifs(os);
            calc_size const cs(os);

            os << std::hex << std::setfill('0') << std::setw(8) << v;
            return cs;
        }

        std::size_t serializer<std::uint64_t, PLAIN>::write(std::ostream &os, std::uint64_t v) {
            //	store and reset stream state
            boost::io::ios_flags_saver ifs(os);
            calc_size const cs(os);

            os << std::hex << std::setfill('0') << std::setw(16) << v;
            return cs;
        }

#if __cplusplus >= 202002L
        std::size_t serializer<std::u8string, PLAIN>::write(std::ostream &os, std::u8string const &s) {
            calc_size const cs(os);
            os << "ToDo";
            return cs;
        }
#endif

        std::size_t serializer<std::u16string, PLAIN>::write(std::ostream &os, std::u16string const &s) {
            calc_size const cs(os);
            //  ToDo:
            os << "ToDo";
            return cs;
        }
        std::size_t serializer<std::u32string, PLAIN>::write(std::ostream &os, std::u32string const &s) {
            calc_size const cs(os);
            cyng::utf8::u32_to_u8_iterator<std::u32string::const_iterator> start(s.begin());
            cyng::utf8::u32_to_u8_iterator<std::u32string::const_iterator> end(s.end());
            os << std::string(start, end);
            return cs;
        }

        std::size_t serializer<double, PLAIN>::write(std::ostream &os, double v) {
            //	store and reset stream state
            boost::io::ios_flags_saver ifs(os);
            calc_size const cs(os);

            os << std::setprecision(2) << std::fixed << v;
            return cs;
        }

    } // namespace io
} // namespace cyng
