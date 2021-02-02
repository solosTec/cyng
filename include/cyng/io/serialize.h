/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_IO_SERIALIZE_H
#define DOCC_IO_SERIALIZE_H

#include <ostream>

namespace docscript {
	class object;
	namespace io {

		//
		//	serialization tags
		//

		/**
		 * Simple serialization to human readable text
		 * struct PLAIN {};
		 */
		void serialize_plain(std::ostream&, object const&);
		std::string to_plain(object const&);

		/**
		 * Serialization to readable output
		 * with type information
		 * struct TYPED {};
		 */
		void serialize_typed(std::ostream&, object const&);
		std::string to_typed(object const&);

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
		//struct JSON_PRETTY {};
		 */
		void serialize_json(std::ostream&, object const&);
		std::string to_json(object const&);

		/**
		 * Serialization to XML.
		 * struct XML {};
		 */
		void serialize_xml(std::ostream&, object const&);
		std::string to_xml(object const&);

		/**
		 * Serialization to CSV.
		 * struct CSV {};
		 */
		void serialize_csv(std::ostream&, object const&);
		std::string to_csv(object const&);

	}
}
#endif
