
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#include "test-xml-001.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/xml.h>
#include <cyng/object.h>
#include <cyng/factory.h>
#include <cyng/io/serializer.h>
#include <boost/uuid/random_generator.hpp>
#include <boost/filesystem.hpp>

namespace cyng 
{

	bool test_xml_001()
	{
#if	CYNG_PUGIXML_INSTALLED == 1
		const boost::filesystem::path tmp = boost::filesystem::temp_directory_path();
		const boost::filesystem::path pwd = boost::filesystem::current_path();
		boost::uuids::random_generator rgen;

		//
		//	create an object tree
		//
		const auto conf = cyng::vector_factory({
			cyng::tuple_factory(cyng::param_factory("log-dir", tmp.string())
			, cyng::param_factory("log-level", "INFO")
				, cyng::param_factory("tag", rgen())
				, cyng::param_factory("generated", std::chrono::system_clock::now())
				, cyng::param_factory("log-pushdata", false)	//	log file for each channel
				, cyng::param_factory("output", cyng::vector_factory({ "DB", "XML", "LOG" }))	//	options are XML, JSON, DB

				, cyng::param_factory("DB", cyng::tuple_factory(
					cyng::param_factory("type", "SQLite"),
					cyng::param_factory("file-name", (pwd / "store.database").string()),
					cyng::param_factory("busy-timeout", 12),		//	seconds
					cyng::param_factory("watchdog", 30),		//	for database connection
					cyng::param_factory("pool-size", 1)		//	no pooling for SQLite
				))
				, cyng::param_factory("XML", cyng::tuple_factory(
					cyng::param_factory("root-dir", (pwd / "xml").string()),
					cyng::param_factory("root-name", "SML"),
					cyng::param_factory("endcoding", "UTF-8")
				))
				, cyng::param_factory("JSON", cyng::tuple_factory(
					cyng::param_factory("root-dir", (pwd / "json").string())
				))
				, cyng::param_factory("ABL", cyng::tuple_factory(
					cyng::param_factory("root-dir", (pwd / "abl").string()),
					cyng::param_factory("prefix", "smf")
				))
				, cyng::param_factory("BIN", cyng::tuple_factory(
					cyng::param_factory("root-dir", (pwd / "sml").string()),
					cyng::param_factory("prefix", "smf"),
					cyng::param_factory("suffix", "sml")
				))
				, cyng::param_factory("LOG", cyng::tuple_factory(
					cyng::param_factory("root-dir", (pwd / "log").string()),
					cyng::param_factory("prefix", "smf")
				))
				, cyng::param_factory("ipt", cyng::vector_factory({
					cyng::tuple_factory(
						cyng::param_factory("host", "127.0.0.1"),
						cyng::param_factory("service", "26862"),
						cyng::param_factory("account", "data-store"),
						cyng::param_factory("pwd", "to-define"),
						cyng::param_factory("def-sk", "0102030405060708090001020304050607080900010203040506070809000001"),	//	scramble key
						cyng::param_factory("scrambled", true),
						cyng::param_factory("monitor", 57)),	//	seconds
					cyng::tuple_factory(
						cyng::param_factory("host", "127.0.0.1"),
						cyng::param_factory("service", "26863"),
						cyng::param_factory("account", "data-store"),
						cyng::param_factory("pwd", "to-define"),
						cyng::param_factory("def-sk", "0102030405060708090001020304050607080900010203040506070809000001"),	//	scramble key
						cyng::param_factory("scrambled", false),
						cyng::param_factory("monitor", 57))
					}))
				, cyng::param_factory("targets", cyng::vector_factory({ "data.sink.1", "data.sink.2" }))	//	list of targets
			)
		});
		//std::cout << io::to_str(conf) << std::endl;

		pugi::xml_document doc;
		auto declarationNode = doc.append_child(pugi::node_declaration);
		declarationNode.append_attribute("version") = "1.0";
		declarationNode.append_attribute("encoding") = "UTF-8";
		declarationNode.append_attribute("standalone") = "yes";

		pugi::xml_node root = doc.append_child("test");
		root.append_attribute("xmlns:xsi") = "w3.org/2001/XMLSchema-instance";

		//
		//	serialize to XML
		//
		xml::write(root, make_object(conf));

		const auto p = boost::filesystem::temp_directory_path() / boost::filesystem::unique_path("unit-test-%%%%-%%%%-%%%%-%%%%.xml");
		if (doc.save_file(p.c_str(), PUGIXML_TEXT("  ")))
		{
			std::cout << p << std::endl;

			doc.reset();
		}

		//
		//	deserialize from XML
		//
		object obj = xml::read_file(p.string());
		//std::cout << io::to_str(obj) << std::endl;

#endif
		return true;
	}
	
}
