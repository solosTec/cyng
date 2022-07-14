
#include <cyng/xml/reader.hpp>
#include <cyng/obj/intrinsics/container.h>
#include <cyng/obj/factory.hpp>
#include <cyng/obj/container_factory.hpp>

#include <cyng/parse/timestamp.h>
#include <cyng/parse/version.h>
#include <cyng/log/conv.h>
//#include <cyng/parser/buffer_parser.h>
#include <cyng/parse/mac.h>
//#include <cyng/intrinsics/eod.h>
//#include <cyng/factory.h>

#include <boost/uuid/string_generator.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/address.hpp>

namespace cyng
{
	namespace xml
	{
		cyng::object read_root(pugi::xml_node node)
		{
			auto type = node.attribute("type").as_string();
			//std::cout << type << std::endl;
			if (boost::algorithm::equals(type, "vec")) {
				vector_t vec;
				vec.reserve(node.attribute("size").as_int());
				for (pugi::xml_node elem : node) {

					//std::cout << elem.name() << std::endl;
					auto elem_type = elem.attribute("type").as_string();

					vec.push_back(is_set(elem_type)
						? read(elem)
						: produce_object(elem_type, elem.child_value()))
						;

				}
				return make_object(vec);
			}
			else if (boost::algorithm::equals(type, "tpl")) {
				tuple_t tpl;
				for (pugi::xml_node elem : node) {

					//std::cout << elem.name() << std::endl;
					auto elem_type = elem.attribute("type").as_string();

					tpl.push_back(is_set(elem_type)
						? read(elem)
						: produce_object(elem_type, elem.child_value()))
						;
				}
				return make_object(tpl);
			}
			return read(node);
		}

		cyng::object read(pugi::xml_node node)
		{
			auto name = node.name();
			auto type = node.attribute("type").as_string();
			//std::cout << name << ", " << type << std::endl;

			if (boost::algorithm::equals(name, "element")) {
				//std::cout
				//	<< name
				//	<< " subtype: "
				//	<< type
				//	<< " size: "
				//	<< node.attribute("size").as_int()
				//	<< std::endl;

				if (boost::algorithm::equals(type, "tpl")) {

					tuple_t tpl;
					for (pugi::xml_node elem : node) {
						//std::cout << elem.name() << std::endl;
						tpl.push_back(read(elem));
					}
					return make_object(tpl);
				}
				else {
					return make_object();

				}

			}
			else if (boost::algorithm::equals(name, "vec")) {
				//std::cout
				//	<< node.name()
				//	<< " type: "
				//	<< type
				//	<< " size: "
				//	<< node.attribute("size").as_int()
				//	<< std::endl;

				vector_t vec;
				vec.reserve(node.attribute("size").as_int());
				for (pugi::xml_node elem : node) {
					//std::cout << elem.name() << std::endl;
					vec.push_back(read(elem));
				}

				return make_object(vec);

			}
			else if (boost::algorithm::equals(name, "tpl")) {
				//std::cout
				//	<< node.name()
				//	<< " type: "
				//	<< type
				//	<< " size: "
				//	<< node.attribute("size").as_int()
				//	<< std::endl;

				//tuple_t tpl;
				//for (pugi::xml_node elem : node) {
				//	std::cout << elem.name() << std::endl;
				//	tpl.push_back(read(elem));
				//}
				//return make_object(tpl);

			}
			else if (boost::algorithm::equals(name, "param")) {
				auto param_name = node.attribute("name").as_string();
				auto value = node.child_value();
				//std::cout
				//	<< node.name()
				//	<< " type: "
				//	<< type
				//	<< " name: "
				//	<< param_name
				//	<< " = "
				//	<< value
				//	<< std::endl;

				return (is_set(type))
					? pair_factory(param_name, read_root(node))
					: pair_factory(param_name, produce_object(type, value))
					;
			}

			return make_object();
		}

		cyng::object produce_object(std::string type, std::string value)
		{
			if (boost::algorithm::equals(type, "null")) {
				//	explicit null
				return make_object();
			}
			else if (boost::algorithm::equals(type, "bool")) {
				return make_object(boost::algorithm::equals(value, "true"));
			}
			else if (boost::algorithm::equals(type, "char")) {
				return make_object(value);
			}
			else if (boost::algorithm::equals(type, "f")) {
				//	float
				return make_object(std::stof(value));
			}
			else if (boost::algorithm::equals(type, "d")) {
				//	double
				return make_object(std::stod(value));
			}
			else if (boost::algorithm::equals(type, "ld")) {
				//	long double
				return make_object(std::stold(value));
			}
			else if (boost::algorithm::equals(type, "s")) {
				return make_object(value);
			}
			else if (boost::algorithm::equals(type, "u8")) {
				return make_object(static_cast<std::uint8_t>(std::stoul(value)));
			}
			else if (boost::algorithm::equals(type, "u16")) {
				return make_object(static_cast<std::uint16_t>(std::stoul(value)));
			}
			else if (boost::algorithm::equals(type, "u32")) {
				return make_object(static_cast<std::uint32_t>(std::stoul(value)));
			}
			else if (boost::algorithm::equals(type, "u64")) {
				return make_object(static_cast<std::uint32_t>(std::stoull(value)));
			}
			else if (boost::algorithm::equals(type, "i8")) {
				return make_object(static_cast<std::int8_t>(std::stoi(value)));
			}
			else if (boost::algorithm::equals(type, "i16")) {
				return make_object(static_cast<std::int16_t>(std::stoi(value)));
			}
			else if (boost::algorithm::equals(type, "i32")) {
				return make_object(static_cast<std::int32_t>(std::stoi(value)));
			}
			else if (boost::algorithm::equals(type, "i64")) {
				return make_object(static_cast<std::int64_t>(std::stoll(value)));
			}
			else if (boost::algorithm::equals(type, "chrono:tp")) {
				return make_object(to_timestamp(value));
			}
			//else if (boost::algorithm::equals(type, "chrono:ns")) {
			//}
			else if (boost::algorithm::equals(type, "v")) {
				return make_object(to_version(value));
			}
			else if (boost::algorithm::equals(type, "rev")) {
				return make_object(to_revision(value));
			}
			//else if (boost::algorithm::equals(type, "op")) {
			//	auto r = parse_revision(value);
			//	if (r.second)	return make_object(r.first);
			//}
			else if (boost::algorithm::equals(type, "log:severity")) {
				//	logging::severity,
				return make_object(to_severity(value));
			}

			else if (boost::algorithm::equals(type, "binary")) {
				//	buffer_t
				//std::pair<buffer_t, bool > r = parse_hex_string(value);
				//if (r.second)	return make_object(r.first);
				return make_object("ToDo");
			}
			else if (boost::algorithm::equals(type, "mac48")) {
				return make_object(to_mac48(value));
			}
			else if (boost::algorithm::equals(type, "mac64")) {
				return make_object(to_mac64(value));
			}
			//"#",		//	RGB color-8
			//"#",		//	RGB color-16

			//"crypto:MD5",
			//"crypto:SHA1",
			//"crypto:SHA256",
			//"crypto:SHA512",
//			"ec",		//	boost::system::error_code
			else if (boost::algorithm::equals(type, "uuid")) {
				return make_object(boost::uuids::string_generator()(value));
			}
			else if (boost::algorithm::equals(type, "fs:path")) {
				return make_object<std::filesystem::path>(value);
			}
//			
//			"ip:tcp:endpoint",	//	boost::asio::ip::tcp::endpoint
//			"ip:udp:endpoint",	//	boost::asio::ip::udp::endpoint
//			"ip:icmp:endpoint",	//	boost::asio::ip::icmp::endpoint
			else if (boost::algorithm::equals(type, "ip:address")) {
				boost::system::error_code ec;
				return make_object(boost::asio::ip::make_address(value, ec));
			}
//
//#if CYNG_ODBC_INSTALLED == 1
//			"sql::tp",
//#else
//			"sql::dummy",
//#endif
//
			else if (boost::algorithm::equals(type, "eod")) {
				return make_eod();
			}

			return make_object();

		}

		bool is_set(std::string type)
		{
			if (boost::algorithm::equals(type, "tpl"))	return true;
			if (boost::algorithm::equals(type, "vec")) return true;
			if (boost::algorithm::equals(type, "set")) return true;
			if (boost::algorithm::equals(type, "amap")) return true;
			if (boost::algorithm::equals(type, "attr")) return true;
			if (boost::algorithm::equals(type, "pmap")) return true;
			if (boost::algorithm::equals(type, "param")) return true;
			return false;
		}


	}
}
