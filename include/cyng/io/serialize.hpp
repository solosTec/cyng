/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_IO_SERIALIZE_HPP
#define CYNG_IO_SERIALIZE_HPP

#include <cyng/io/io.h>
#include <cyng/obj/object.h>
#include <cyng/obj/object_cast.hpp>
#include <cyng/obj/tag.hpp>
#include <cyng/obj/type_interface.h>

#include <ostream>

#include <cyng/io/serializer/binary.hpp>
#include <cyng/io/serializer/cpp.hpp>
#include <cyng/io/serializer/csv.hpp>
#include <cyng/io/serializer/json.hpp>
#include <cyng/io/serializer/plain.hpp>
#include <cyng/io/serializer/typed.hpp>

namespace cyng {
    namespace io {

        template <typename S> struct serialize {
            template <typename T> static std::size_t write_impl(std::ostream &os, object const &obj) {
                //
                //	select a serializer
                //
                using serial_t = serializer<T, S>;
                auto ptr = object_cast<T>(obj);
                return (ptr != nullptr) ? serial_t::write(os, *ptr) : 0;
            }

            static std::size_t write(std::ostream &os, object const &obj) {
                if (!obj) {
                    return 0;
                } else {

                    //
                    //	Runtime meets compile time.
                    //	This switch converts the type data available at runtime
                    //	in serialization functions selected from the compiler
                    //
                    switch (obj.tag()) {
                    case TC_NULL: return write_impl<traits::reverse_type<TC_NULL>::type>(os, obj);
                    case TC_BOOL: return write_impl<traits::reverse_type<TC_BOOL>::type>(os, obj);
                    case TC_CHAR: return write_impl<traits::reverse_type<TC_CHAR>::type>(os, obj);
                    case TC_FLOAT: return write_impl<traits::reverse_type<TC_FLOAT>::type>(os, obj);
                    case TC_DOUBLE: return write_impl<traits::reverse_type<TC_DOUBLE>::type>(os, obj);
                    case TC_FLOAT80: return write_impl<traits::reverse_type<TC_FLOAT80>::type>(os, obj);

                    case TC_UINT8: return write_impl<traits::reverse_type<TC_UINT8>::type>(os, obj);
                    case TC_UINT16: return write_impl<traits::reverse_type<TC_UINT16>::type>(os, obj);
                    case TC_UINT32: return write_impl<traits::reverse_type<TC_UINT32>::type>(os, obj);
                    case TC_UINT64: return write_impl<traits::reverse_type<TC_UINT64>::type>(os, obj);
                    case TC_INT8: return write_impl<traits::reverse_type<TC_INT8>::type>(os, obj);
                    case TC_INT16: return write_impl<traits::reverse_type<TC_INT16>::type>(os, obj);
                    case TC_INT32: return write_impl<traits::reverse_type<TC_INT32>::type>(os, obj);
                    case TC_INT64: return write_impl<traits::reverse_type<TC_INT64>::type>(os, obj);

                    case TC_STRING:
                        return write_impl<traits::reverse_type<TC_STRING>::type>(os, obj);
                        //                    case TC_STRING_U8: return write_impl<traits::reverse_type<TC_STRING_U8>::type>(os,
                        //                    obj);
                    case TC_STRING_U16: return write_impl<traits::reverse_type<TC_STRING_U16>::type>(os, obj);
                    case TC_STRING_U32: return write_impl<traits::reverse_type<TC_STRING_U32>::type>(os, obj);
                    case TC_FS_PATH: return write_impl<traits::reverse_type<TC_FS_PATH>::type>(os, obj);

                    case TC_TIME_POINT: return write_impl<traits::reverse_type<TC_TIME_POINT>::type>(os, obj);
                    case TC_NANO_SECOND: return write_impl<traits::reverse_type<TC_NANO_SECOND>::type>(os, obj);
                    case TC_MICRO_SECOND: return write_impl<traits::reverse_type<TC_MICRO_SECOND>::type>(os, obj);
                    case TC_MILLI_SECOND: return write_impl<traits::reverse_type<TC_MILLI_SECOND>::type>(os, obj);
                    case TC_SECOND: return write_impl<traits::reverse_type<TC_SECOND>::type>(os, obj);
                    case TC_MINUTE: return write_impl<traits::reverse_type<TC_MINUTE>::type>(os, obj);
                    case TC_HOUR: return write_impl<traits::reverse_type<TC_HOUR>::type>(os, obj);

                    case TC_BUFFER: return write_impl<traits::reverse_type<TC_BUFFER>::type>(os, obj);
                    case TC_VERSION: return write_impl<traits::reverse_type<TC_VERSION>::type>(os, obj);
                    case TC_REVISION: return write_impl<traits::reverse_type<TC_REVISION>::type>(os, obj);
                    case TC_OP: return write_impl<traits::reverse_type<TC_OP>::type>(os, obj);
                    case TC_SEVERITY: return write_impl<traits::reverse_type<TC_SEVERITY>::type>(os, obj);
                    case TC_MAC48: return write_impl<traits::reverse_type<TC_MAC48>::type>(os, obj);
                    case TC_MAC64: return write_impl<traits::reverse_type<TC_MAC64>::type>(os, obj);

                    case TC_PID: return write_impl<traits::reverse_type<TC_PID>::type>(os, obj);
                    case TC_OBIS: return write_impl<traits::reverse_type<TC_OBIS>::type>(os, obj);
                    case TC_OBISPATH: return write_impl<traits::reverse_type<TC_OBISPATH>::type>(os, obj);
                    case TC_EDIS: return write_impl<traits::reverse_type<TC_EDIS>::type>(os, obj);
                    case TC_COLOR_8: return write_impl<traits::reverse_type<TC_COLOR_8>::type>(os, obj);
                    case TC_COLOR_16: return write_impl<traits::reverse_type<TC_COLOR_16>::type>(os, obj);

                    case TC_DIGEST_MD5: return write_impl<traits::reverse_type<TC_DIGEST_MD5>::type>(os, obj);
                    case TC_DIGEST_SHA1: return write_impl<traits::reverse_type<TC_DIGEST_SHA1>::type>(os, obj);
                    case TC_DIGEST_SHA256: return write_impl<traits::reverse_type<TC_DIGEST_SHA256>::type>(os, obj);
                    case TC_DIGEST_SHA512: return write_impl<traits::reverse_type<TC_DIGEST_SHA512>::type>(os, obj);

                    case TC_AES128: return write_impl<traits::reverse_type<TC_AES128>::type>(os, obj);
                    case TC_AES192: return write_impl<traits::reverse_type<TC_AES192>::type>(os, obj);
                    case TC_AES256: return write_impl<traits::reverse_type<TC_AES256>::type>(os, obj);

                    case TC_OBJECT: BOOST_ASSERT_MSG(false, "nested object"); return 0;

                    case TC_RAW: return write_impl<traits::reverse_type<TC_RAW>::type>(os, obj);
                    case TC_DATE: return write_impl<traits::reverse_type<TC_DATE>::type>(os, obj);
                    case TC_TIME: return write_impl<traits::reverse_type<TC_TIME>::type>(os, obj);

                    case TC_TUPLE: return write_impl<traits::reverse_type<TC_TUPLE>::type>(os, obj);
                    case TC_VECTOR: return write_impl<traits::reverse_type<TC_VECTOR>::type>(os, obj);
                    case TC_DEQUE: return write_impl<traits::reverse_type<TC_DEQUE>::type>(os, obj);

                    case TC_ATTR_MAP: return write_impl<traits::reverse_type<TC_ATTR_MAP>::type>(os, obj);
                    case TC_ATTR: return write_impl<traits::reverse_type<TC_ATTR>::type>(os, obj);
                    case TC_PARAM_MAP: return write_impl<traits::reverse_type<TC_PARAM_MAP>::type>(os, obj);
                    case TC_PARAM: return write_impl<traits::reverse_type<TC_PARAM>::type>(os, obj);
                    case TC_PROP_MAP: return write_impl<traits::reverse_type<TC_PROP_MAP>::type>(os, obj);
                    case TC_PROP: return write_impl<traits::reverse_type<TC_PROP>::type>(os, obj);

                    case TC_PRG: return write_impl<traits::reverse_type<TC_PRG>::type>(os, obj);

                    case TC_EC: return write_impl<traits::reverse_type<TC_EC>::type>(os, obj);
                    case TC_UUID: return write_impl<traits::reverse_type<TC_UUID>::type>(os, obj);

                    case TC_IP_ADDRESS: return write_impl<traits::reverse_type<TC_IP_ADDRESS>::type>(os, obj);
                    case TC_IP_TCP_ENDPOINT: return write_impl<traits::reverse_type<TC_IP_TCP_ENDPOINT>::type>(os, obj);
                    case TC_IP_UDP_ENDPOINT: return write_impl<traits::reverse_type<TC_IP_UDP_ENDPOINT>::type>(os, obj);
                    case TC_IP_ICMP_ENDPOINT: return write_impl<traits::reverse_type<TC_IP_ICMP_ENDPOINT>::type>(os, obj);

                    case TC_EOD: return write_impl<traits::reverse_type<TC_EOD>::type>(os, obj); ;

                    default: BOOST_ASSERT_MSG(false, "unknown data type to serialize"); break;
                    }

                    return 0;
                }
            }
        };
    } // namespace io
} // namespace cyng
#endif
