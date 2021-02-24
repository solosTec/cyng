/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#include <cyng/io/serialize.h>
#include <cyng/io/serialize.hpp>
#include <cyng/io/io.h>
#include <cyng/obj/object.h>
#include <cyng/io/serializer/json_walker.h>

namespace cyng {
	namespace io {

		void serialize_plain(std::ostream& os, object const& obj)
		{
			serialize<PLAIN>::write(os, obj);
		}

		std::string to_plain(object const& obj)
		{
			std::stringstream ss;
			serialize_plain(ss, obj);
			return ss.str();
		}


		void serialize_typed(std::ostream& os, object const& obj)
		{
			serialize<TYPED>::write(os, obj);
		}

		std::string to_typed(object const& obj)
		{
			std::stringstream ss;
			serialize_typed(ss, obj);
			return ss.str();
		}

		std::size_t serialize_binary(std::ostream& os, object const& obj)
		{
			return serialize<BINARY>::write(os, obj);
		}

		void serialize_json(std::ostream& os, object const& obj)
		{
			serialize<JSON>::write(os, obj);
		}

		std::string to_json(object const& obj)
		{
			std::stringstream ss;
			serialize_json(ss, obj);
			return ss.str();
		}

		void serialize_json(std::ostream& os, param_map_t const& pmap) {
			serializer <param_map_t, JSON>::write(os, pmap);
		}

		std::string to_json(param_map_t const& pmap) {
			std::stringstream ss;
			serialize_json(ss, pmap);
			return ss.str();
		}

		void serialize_json_pretty(std::ostream& os, object const& obj) {
			json_walker walker(os);
			traverse(obj, walker);
		}

		std::string to_json_pretty(object const& obj) {
			std::stringstream ss;
			serialize_json_pretty(ss, obj);
			return ss.str();
		}

		void serialize_xml(std::ostream& os, object const& obj)
		{
			serialize<XML>::write(os, obj);
		}

		std::string to_xml(object const& obj)
		{
			std::stringstream ss;
			serialize_xml(ss, obj);
			return ss.str();
		}

		void serialize_csv(std::ostream& os, object const& obj)
		{
			serialize<CSV>::write(os, obj);
		}

		std::string to_csv(object const& obj)
		{
			std::stringstream ss;
			serialize_csv(ss, obj);
			return ss.str();
		}

	}
}
