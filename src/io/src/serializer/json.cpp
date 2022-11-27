/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#include <cyng/io/ostream.h>
#include <cyng/io/parser/utf-8.hpp>
#include <cyng/io/serialize.h>
#include <cyng/io/serializer/json.hpp>
#include <cyng/io/serializer/plain.hpp>
#include <cyng/obj/tag.hpp>

#include <iomanip>

#include <boost/io/ios_state.hpp>

namespace cyng {
    namespace io {

        std::size_t serializer<bool, JSON>::write(std::ostream &os, bool v) { return serializer<bool, PLAIN>::write(os, v); }

        std::size_t serializer<null, JSON>::write(std::ostream &os, null n) {
            calc_size const cs(os);
            os << n;
            return cs;
        }

        std::size_t serializer<vector_t, JSON>::write(std::ostream &os, vector_t const &vec) {
            calc_size const cs(os);

            if (vec.empty()) {
                os << "null";
            } else {
                os << '[';

                //	serialize each element from the tuple
                bool init = false;

                for (auto const &obj : vec) {
                    if (!init) {
                        init = true;
                    } else {
                        os << ", ";
                    }
                    auto const tag = obj.tag();
                    switch (tag) {
                    case TC_ATTR:
                    case TC_PARAM:
                        os << '{';
                        serialize_json(os, obj);
                        os << '}';
                        break;
                    default: serialize_json(os, obj);
                    }
                }

                os << ']';
            }

            return cs;
        }

        std::size_t serializer<prg_t, JSON>::write(std::ostream &os, prg_t const &prg) {
            calc_size const cs(os);

            if (prg.empty()) {
                os << "null";
            } else {
                os << '[';

                //	serialize each element from the tuple
                bool init = false;

                for (auto const &buf : prg) {
                    if (!init) {
                        init = true;
                    } else {
                        os << ", ";
                    }
                    serializer<buffer_t, JSON>::write(os, buf);
                }

                os << ']';
            }

            return cs;
        }

        std::size_t serializer<tuple_t, JSON>::write(std::ostream &os, tuple_t const &tpl) {

            calc_size const cs(os);

            if (tpl.empty()) {
                os << "null";
            } else {
                os << '{';

                //	serialize each element from the tuple
                bool init = false;
                for (auto const &obj : tpl) {
                    if (!init) {
                        init = true;
                    } else {
                        os << ", ";
                    }
                    serialize_json(os, obj);
                }

                os << '}';
            }

            return cs;
        }

        std::size_t serializer<param_t, JSON>::write(std::ostream &os, param_t const &param) {

            calc_size const cs(os);

            os << '"' << param.first << '"' << ':' << ' ';

            auto const tag = param.second.tag();
            switch (tag) {
            case TC_ATTR:
            case TC_PARAM:
                os << '{';
                serialize_json(os, param.second);
                os << '}';
                break;
            default: serialize_json(os, param.second);
            }

            return cs;
        }

        std::size_t serializer<param_map_t, JSON>::write(std::ostream &os, param_map_t const &pm) {

            calc_size const cs(os);

            if (pm.empty()) {
                os << "null";
            } else {
                os << '{';

                bool flag = false;
                for (auto const &p : pm) {
                    if (flag) {
                        os << ',';
                    } else {
                        flag = true;
                    }

                    os << '"' << p.first << '"' << ':';

                    serialize_json(os, p.second);
                }
                os << '}';
            }

            return cs;
        }

        std::size_t serializer<prop_t, JSON>::write(std::ostream &os, prop_t const &prop) {

            calc_size const cs(os);

            os << '"' << to_string(prop.first) << '"' << ':' << ' ';

            auto const tag = prop.second.tag();
            switch (tag) {
            case TC_ATTR:
            case TC_PARAM:
                os << '{';
                serialize_json(os, prop.second);
                os << '}';
                break;
            default: serialize_json(os, prop.second);
            }

            return cs;
        }

        std::size_t serializer<prop_map_t, JSON>::write(std::ostream &os, prop_map_t const &pm) {

            calc_size const cs(os);

            if (pm.empty()) {
                os << "null";
            } else {
                os << '{';

                bool flag = false;
                for (auto const &p : pm) {
                    if (flag) {
                        os << ',';
                    } else {
                        flag = true;
                    }

                    os << '"' << p.first << '"' << ':';

                    serialize_json(os, p.second);
                }
                os << '}';
            }

            return cs;
        }

        std::size_t serializer<std::int8_t, JSON>::write(std::ostream &os, std::int8_t v) {
            return serializer<std::int8_t, PLAIN>::write(os, v);
        }
        std::size_t serializer<std::int16_t, JSON>::write(std::ostream &os, std::int16_t v) {
            return serializer<std::int16_t, PLAIN>::write(os, v);
        }
        std::size_t serializer<std::int32_t, JSON>::write(std::ostream &os, std::int32_t v) {
            return serializer<std::int32_t, PLAIN>::write(os, v);
        }
        std::size_t serializer<std::int64_t, JSON>::write(std::ostream &os, std::int64_t v) {
            return serializer<std::int64_t, PLAIN>::write(os, v);
        }
        std::size_t serializer<std::uint8_t, JSON>::write(std::ostream &os, std::uint8_t v) {
            calc_size const cs(os);
            boost::io::ios_flags_saver ifs(os);

            os << std::dec << +v;

            return cs;
        }
        std::size_t serializer<std::uint16_t, JSON>::write(std::ostream &os, std::uint16_t v) {
            calc_size const cs(os);
            boost::io::ios_flags_saver ifs(os);

            os << std::dec << v;

            return cs;
        }
        std::size_t serializer<std::uint32_t, JSON>::write(std::ostream &os, std::uint32_t v) {
            calc_size const cs(os);
            boost::io::ios_flags_saver ifs(os);

            os << std::dec << v;

            return cs;
        }
        std::size_t serializer<std::uint64_t, JSON>::write(std::ostream &os, std::uint64_t v) {
            calc_size const cs(os);
            boost::io::ios_flags_saver ifs(os);

            os << std::dec << v;

            return cs;
        }
        std::size_t serializer<double, JSON>::write(std::ostream &os, double v) { return serializer<double, PLAIN>::write(os, v); }
        std::size_t serializer<std::string, JSON>::write(std::ostream &os, std::string const &str) {
            calc_size const cs(os);
            //	store and reset stream state
            boost::io::ios_flags_saver ifs(os);

            os << std::setfill('0') << std::hex << '"';

            utf8::u8_to_u32_iterator begin(str.begin());
            utf8::u8_to_u32_iterator end(str.end());
            for (; begin != end; ++begin) {
                auto const c = *begin;

                switch (c) {
                case '\a': os << '\\' << 'a'; break;
                case '\b': os << '\\' << 'b'; break;
                case '\f': os << '\\' << 'f'; break;
                case '\n': os << '\\' << 'n'; break;
                case '\r': os << '\\' << 'r'; break;
                case '\t': os << '\\' << 't'; break;
                case '\v': os << '\\' << 'v'; break;
                case '\\': os << '\\' << '\\'; break;
                case '\'': os << '\\' << '\''; break;
                case '\"': os << '\\' << '\"'; break;
                default:
                    if (c > 30 && c < 127) {
                        os << (char)c;
                    } else {
                        //	escape other non-printable characters
                        os << "\\u" << std::setw(4) << std::setfill('0') << c;
                    }
                    break;
                }
            }

            os << '"';

            return cs;
        }

        std::size_t serializer<buffer_t, JSON>::write(std::ostream &os, buffer_t const &buffer) {
            calc_size const cs(os);
            //	store and reset stream state
            boost::io::ios_flags_saver ifs(os);
            os << '"' << buffer << '"';

            return cs;
        }

        std::size_t serializer<std::chrono::system_clock::time_point, JSON>::write(
            std::ostream &os,
            std::chrono::system_clock::time_point const &v) {

            calc_size const cs(os);
            std::time_t const tt = std::chrono::system_clock::to_time_t(v);

            //
            //	the generic "Jan. 1 1970" is null
            //
            if (0 == tt) {
                os << "null";
            } else {
                auto tm = *std::gmtime(&tt);
                //	example: 1995-12-04T00:12:00-0430
                // ISO 8601
                os << '"' << std::put_time(&tm, "%FT%T") << '"';
            }

            return cs;
        }

        std::size_t serializer<std::filesystem::path, JSON>::write(std::ostream &os, std::filesystem::path const &v) {

            calc_size const cs(os);
            auto const str = v.string();
            if (str.empty()) {
                os << std::string(2, '"');
            } else {
                //
                //  This solves a problem with different conversions of native and generic paths
                //
                if (str.at(0) == '"') {
                    os << v.string();
                } else {
                    os << '"' << v.string() << '"';
                }
            }
            return cs;
        }

        std::size_t serializer<severity, JSON>::write(std::ostream &os, severity s) {

            calc_size const cs(os);

            os << '"';
            switch (s) {
            case severity::LEVEL_TRACE: os << "TRACE"; break;
            case severity::LEVEL_DEBUG: os << "DEBUG"; break;
            case severity::LEVEL_INFO: os << "INFO"; break;
            case severity::LEVEL_WARNING: os << "WARNING"; break;
            case severity::LEVEL_ERROR: os << "ERROR"; break;
            case severity::LEVEL_FATAL: os << "FATAL"; break;
            default: break;
            }
            os << '"';

            return cs;
        }
    } // namespace io
} // namespace cyng
