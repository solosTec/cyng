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

		std::size_t serialize_plain(std::ostream& os, object const& obj)
		{
			return serialize<PLAIN>::write(os, obj);
		}

		std::string to_plain(object const& obj)
		{
			std::stringstream ss;
			serialize_plain(ss, obj);
			return ss.str();
		}


		std::size_t serialize_typed(std::ostream& os, object const& obj)	{
			return serialize<TYPED>::write(os, obj);
		}

		std::size_t serialize_typed(std::ostream& os, vector_t const& vec) {
			return serializer <vector_t, TYPED>::write(os, vec);
		}

		std::string to_typed(object const& obj)
		{
			std::stringstream ss;
			serialize_typed(ss, obj);
			return ss.str();
		}

		std::string to_typed(vector_t const& vec) {
			std::stringstream ss;
			serialize_typed(ss, vec);
			return ss.str();
		}


		std::size_t serialize_binary(std::ostream& os, object const& obj)
		{
			return serialize<BINARY>::write(os, obj);
		}

		std::size_t serialize_json(std::ostream& os, object const& obj)
		{
			return serialize<JSON>::write(os, obj);
		}

		std::string to_json(object const& obj)
		{
			std::stringstream ss;
			serialize_json(ss, obj);
			return ss.str();
		}

		std::size_t serialize_json(std::ostream& os, param_map_t const& pmap) {
			return serializer <param_map_t, JSON>::write(os, pmap);
		}

		std::size_t serialize_json(std::ostream& os, tuple_t const& tpl) {
			return serializer <tuple_t, JSON>::write(os, tpl);
		}

		std::string to_json(param_map_t const& pmap) {
			std::stringstream ss;
			serialize_json(ss, pmap);
			return ss.str();
		}

		std::string to_json(tuple_t const& tpl) {
			std::stringstream ss;
			serialize_json(ss, tpl);
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

		std::size_t serialize_xml(std::ostream& os, object const& obj)
		{
			return 0;
			//return serialize<XML>::write(os, obj);
		}

		std::string to_xml(object const& obj)
		{
			std::stringstream ss;
			serialize_xml(ss, obj);
			return ss.str();
		}

		std::size_t serialize_csv(std::ostream& os, object const& obj)
		{
			return serialize<CSV>::write(os, obj);
		}

		std::string to_csv(object const& obj)
		{
			std::stringstream ss;
			serialize_csv(ss, obj);
			return ss.str();
		}

	}
}
