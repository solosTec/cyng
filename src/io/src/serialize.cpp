/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#include <cyng/io/serialize.h>

#include <cyng/io/serializer/xml.hpp>
#include <cyng/obj/object.h>

#include <fstream>

namespace cyng {
    namespace io {

        std::size_t serialize_plain(std::ostream &os, object const &obj) { return serialize<PLAIN>::write(os, obj); }

        std::size_t serialize_typed(std::ostream &os, object const &obj) { return serialize<TYPED>::write(os, obj); }

        std::size_t serialize_binary(std::ostream &os, object const &obj) { return serialize<BINARY>::write(os, obj); }

        std::size_t serialize_json(std::ostream &os, object const &obj) { return serialize<JSON>::write(os, obj); }

        std::size_t serialize_xml_native(pugi::xml_node node, object const &obj) { return serialize<XML>::write(node, obj); }

        void serialize_xml(std::ostream &os, object const &obj, std::string root_name) {
            pugi::xml_document doc;
            auto declarationNode = doc.append_child(pugi::node_declaration);
            declarationNode.append_attribute("version") = "1.0";
            declarationNode.append_attribute("encoding") = "UTF-8";
            declarationNode.append_attribute("standalone") = "yes";

            pugi::xml_node root = doc.append_child(root_name.c_str());
            root.append_attribute("xmlns:xsi") = "w3.org/2001/XMLSchema-instance";

            serialize_xml_native(root, obj);
            doc.save(os);
        }

        std::string to_xml(object const &obj, std::string root_name) {
            std::stringstream ss;
            serialize_xml(ss, obj, root_name);
            return ss.str();
        }

        bool to_xml_file(object const &obj, std::string file_name, std::string root_name) {
            std::ofstream of(file_name, std::fstream::app);
            if (of.is_open()) {
                serialize_xml(of, obj, root_name);
                return true;
            }
            return false;
        }

        std::size_t serialize_csv(std::ostream &os, object const &obj) { return serialize<CSV>::write(os, obj); }

        std::string to_csv(object const &obj) {
            std::stringstream ss;
            serialize_csv(ss, obj);
            return ss.str();
        }

        bool serialize<XML>::write(pugi::xml_node node, object const &obj) {
            if (!obj) {
                return false;
            }
            //
            //	Runtime meets compile time.
            //	This switch converts the type data available at runtime
            //	in serialization functions selected from the compiler
            //
            switch (obj.tag()) {
            case TC_NULL: return write_impl<traits::reverse_type<TC_NULL>::type>(node, obj);
            case TC_BOOL: return write_impl<traits::reverse_type<TC_BOOL>::type>(node, obj);
            case TC_CHAR: return write_impl<traits::reverse_type<TC_CHAR>::type>(node, obj);
            case TC_FLOAT: return write_impl<traits::reverse_type<TC_FLOAT>::type>(node, obj);
            case TC_DOUBLE:
                return write_impl<traits::reverse_type<TC_DOUBLE>::type>(node, obj);
                // case TC_FLOAT80:	return write_impl<traits::reverse_type<TC_FLOAT80>::type>(node, obj);

            case TC_UINT8: return write_impl<traits::reverse_type<TC_UINT8>::type>(node, obj);
            case TC_UINT16: return write_impl<traits::reverse_type<TC_UINT16>::type>(node, obj);
            case TC_UINT32: return write_impl<traits::reverse_type<TC_UINT32>::type>(node, obj);
            case TC_UINT64: return write_impl<traits::reverse_type<TC_UINT64>::type>(node, obj);
            case TC_INT8: return write_impl<traits::reverse_type<TC_INT8>::type>(node, obj);
            case TC_INT16: return write_impl<traits::reverse_type<TC_INT16>::type>(node, obj);
            case TC_INT32: return write_impl<traits::reverse_type<TC_INT32>::type>(node, obj);
            case TC_INT64: return write_impl<traits::reverse_type<TC_INT64>::type>(node, obj);

            case TC_STRING: return write_impl<traits::reverse_type<TC_STRING>::type>(node, obj);
            case TC_STRING_U8: return write_impl<traits::reverse_type<TC_STRING_U8>::type>(node, obj);
            case TC_STRING_U16: return write_impl<traits::reverse_type<TC_STRING_U16>::type>(node, obj);
            case TC_STRING_U32: return write_impl<traits::reverse_type<TC_STRING_U32>::type>(node, obj);
            case TC_FS_PATH: return write_impl<traits::reverse_type<TC_FS_PATH>::type>(node, obj);

            case TC_TIME_POINT: return write_impl<traits::reverse_type<TC_TIME_POINT>::type>(node, obj);
            case TC_NANO_SECOND: return write_impl<traits::reverse_type<TC_NANO_SECOND>::type>(node, obj);
            case TC_MICRO_SECOND: return write_impl<traits::reverse_type<TC_MICRO_SECOND>::type>(node, obj);
            case TC_MILLI_SECOND: return write_impl<traits::reverse_type<TC_MILLI_SECOND>::type>(node, obj);
            case TC_SECOND: return write_impl<traits::reverse_type<TC_SECOND>::type>(node, obj);
            case TC_MINUTE: return write_impl<traits::reverse_type<TC_MINUTE>::type>(node, obj);
            case TC_HOUR: return write_impl<traits::reverse_type<TC_HOUR>::type>(node, obj);

            case TC_BUFFER: return write_impl<traits::reverse_type<TC_BUFFER>::type>(node, obj);
            case TC_VERSION: return write_impl<traits::reverse_type<TC_VERSION>::type>(node, obj);
            case TC_REVISION: return write_impl<traits::reverse_type<TC_REVISION>::type>(node, obj);
            // case TC_OP:			return write_impl<traits::reverse_type<TC_OP>::type>(node, obj);
            // case TC_SEVERITY:	return write_impl<traits::reverse_type<TC_SEVERITY>::type>(node, obj);
            case TC_MAC48: return write_impl<traits::reverse_type<TC_MAC48>::type>(node, obj);
            case TC_MAC64: return write_impl<traits::reverse_type<TC_MAC64>::type>(node, obj);

            case TC_PID: return write_impl<traits::reverse_type<TC_PID>::type>(node, obj);
            case TC_OBIS: return write_impl<traits::reverse_type<TC_OBIS>::type>(node, obj);
            case TC_OBISPATH: return write_impl<traits::reverse_type<TC_OBISPATH>::type>(node, obj);
            case TC_EDIS: return write_impl<traits::reverse_type<TC_EDIS>::type>(node, obj);

            case TC_COLOR_8: return write_impl<traits::reverse_type<TC_COLOR_8>::type>(node, obj);
            case TC_COLOR_16: return write_impl<traits::reverse_type<TC_COLOR_16>::type>(node, obj);

            // case TC_DIGEST_MD5:		return write_impl<traits::reverse_type<TC_DIGEST_MD5>::type>(node, obj);
            case TC_DIGEST_SHA1: return write_impl<traits::reverse_type<TC_DIGEST_SHA1>::type>(node, obj);
            case TC_DIGEST_SHA256: return write_impl<traits::reverse_type<TC_DIGEST_SHA256>::type>(node, obj);
            case TC_DIGEST_SHA512: return write_impl<traits::reverse_type<TC_DIGEST_SHA512>::type>(node, obj);

            case TC_AES128: return write_impl<traits::reverse_type<TC_AES128>::type>(node, obj);
            case TC_AES192: return write_impl<traits::reverse_type<TC_AES192>::type>(node, obj);
            case TC_AES256: return write_impl<traits::reverse_type<TC_AES256>::type>(node, obj);

            case TC_OBJECT: BOOST_ASSERT_MSG(false, "nested object"); return 0;

            case TC_TUPLE: return write_impl<traits::reverse_type<TC_TUPLE>::type>(node, obj);
            case TC_VECTOR: return write_impl<traits::reverse_type<TC_VECTOR>::type>(node, obj);
            case TC_DEQUE: return write_impl<traits::reverse_type<TC_DEQUE>::type>(node, obj);

            case TC_ATTR_MAP: return write_impl<traits::reverse_type<TC_ATTR_MAP>::type>(node, obj);
            case TC_ATTR: return write_impl<traits::reverse_type<TC_ATTR>::type>(node, obj);
            case TC_PARAM_MAP: return write_impl<traits::reverse_type<TC_PARAM_MAP>::type>(node, obj);
            case TC_PARAM: return write_impl<traits::reverse_type<TC_PARAM>::type>(node, obj);

            // case TC_EC:			return write_impl<traits::reverse_type<TC_EC>::type>(node, obj);
            case TC_UUID: return write_impl<traits::reverse_type<TC_UUID>::type>(node, obj);

            case TC_IP_ADDRESS: return write_impl<traits::reverse_type<TC_IP_ADDRESS>::type>(node, obj);
            case TC_IP_TCP_ENDPOINT: return write_impl<traits::reverse_type<TC_IP_TCP_ENDPOINT>::type>(node, obj);
            case TC_IP_UDP_ENDPOINT: return write_impl<traits::reverse_type<TC_IP_UDP_ENDPOINT>::type>(node, obj);
            case TC_IP_ICMP_ENDPOINT: return write_impl<traits::reverse_type<TC_IP_ICMP_ENDPOINT>::type>(node, obj);

            case TC_EOD: return write_impl<traits::reverse_type<TC_EOD>::type>(node, obj); ;

            default: BOOST_ASSERT_MSG(false, "unknown data type to serialize"); break;
            }
            return false;
        }

        std::size_t serialize_cpp(std::ostream &os, object const &obj) { return serialize<CPP>::write(os, obj); }

        std::string to_cpp(object const &obj) {
            std::stringstream ss;
            serialize_cpp(ss, obj);
            return ss.str();
        }

    } // namespace io
} // namespace cyng
