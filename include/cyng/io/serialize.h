/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_IO_SERIALIZE_H
#define CYNG_IO_SERIALIZE_H

#include <ostream>
#include <cyng/obj/intrinsics/container.h>

namespace cyng {
	class object;
	namespace io {

		//
		//	serialization tags
		//

		/**
		 * Simple serialization to human readable text
		 * struct PLAIN {};
		 */
		std::size_t serialize_plain(std::ostream&, object const&);
		std::string to_plain(object const&);

		/**
		 * Serialization to readable output
		 * with type information
		 * struct TYPED {};
		 */
		std::size_t serialize_typed(std::ostream&, object const&);
		std::size_t serialize_typed(std::ostream&, vector_t const&);
		std::string to_typed(object const&);
		std::string to_typed(vector_t const&);

		/**
		 * Serialization to an internal binary
		 * format with TLV (type-length-value) encoding.
		 * struct BINARY {};
		 * 
		 * @return bytes that were written
		 */
		std::size_t serialize_binary(std::ostream&, object const&);

		/**
		 * Serialization to JSON.
		 * struct JSON {};
		 */
		std::size_t serialize_json(std::ostream&, object const&);
		std::string to_json(object const&);
		std::size_t serialize_json(std::ostream&, param_map_t const&);
		std::size_t serialize_json(std::ostream&, tuple_t const&);
		std::string to_json(param_map_t const&);
		std::string to_json(tuple_t const&);

		void serialize_json_pretty(std::ostream&, object const&);
		std::string to_json_pretty(object const&);

		/**
		 * Serialization to XML.
		 * struct XML {};
		 */
		std::size_t serialize_xml(std::ostream&, object const&);
		std::string to_xml(object const&);

		/**
		 * Serialization to CSV.
		 * struct CSV {};
		 */
		std::size_t serialize_csv(std::ostream&, object const&);
		std::string to_csv(object const&);
	}
}
#endif
