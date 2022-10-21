#include <cyng/xml/xml.h>
#include <cyng/obj/factory.hpp>
#include <cyng/xml/reader.hpp>

#include <sstream>
#include <pugixml.hpp>

#include <boost/core/ignore_unused.hpp>

namespace cyng 
{
	namespace xml 
	{
		object read_file(std::string const& p)
		{
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(p.c_str());
			if (result) {
	
				auto root = doc.first_child();
				auto type = root.attribute("type").as_string();
				boost::ignore_unused(type);

				return read_root(root);
			}

			std::stringstream ss;
			ss
				<< "XML [" 
				<< p 
				<< "] parsed with errors: [" 
				<< result.description()
				<< "]\n"
				<< "Error offset: " 
				<< result.offset
				;
			return make_object(ss.str());
		}

	//	cyng::record read(pugi::xml_node node, meta_store const& meta)
	//	{
	//		try {
	//			cyng::key_t key;
	//			cyng::data_t data;
	//			std::uint64_t gen{ 0 };

	//			pugi::xml_node const param_key = node.find_child_by_attribute("param", "name", "key");
	//			if (param_key) {

	//				//
	//				//	key
	//				//
	//				meta.loop([&](std::size_t, column const& col, bool pk) {

	//					if (pk) {
	//						auto type = param_key.child(col.name_.c_str()).attribute("type").as_string();
	//						key.push_back(produce_object(type, param_key.child_value(col.name_.c_str())));
	//					}
	//				});

	//			}
	//			pugi::xml_node const param_data = node.find_child_by_attribute("param", "name", "data");
	//			if (param_data) {

	//				//
	//				//	data
	//				//
	//				meta.loop([&](std::size_t, column const& col, bool pk) {

	//					if (!pk) {
	//						BOOST_ASSERT_MSG(!boost::algorithm::equals(col.name_, "gen"), "keyword gen is not allowed in this context");
	//						auto type = param_data.child(col.name_.c_str()).attribute("type").as_string();
	//						data.push_back(produce_object(type, param_data.child_value(col.name_.c_str())));
	//					}

	//				});

	//				//
	//				//	gen(eration)
	//				//
	//				gen = std::stoull(param_data.child_value("gen"));
	//			}

	//			return cyng::record(meta, key, data, gen);
	//		}
	//		catch (std::exception const&) {
	//		}

	//		//
	//		//	return empty record
	//		//
	//		return cyng::record(meta);

	//	}

	}
}

