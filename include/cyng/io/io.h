/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_IO_H
#define CYNG_IO_H

#include <ostream>

namespace cyng {
	namespace io {

		//
		//	serialization tags
		//

		/**
		 * Simple serialization to human readable text
		 */
		struct PLAIN {};

		/**
		 * Serialization to readable output
		 * with type information
		 */
		struct TYPED {};

		/**
		 * Serialization to an internal binary
		 * format with TLV (type-length-value) encoding.
		 */
		struct BINARY {};

		/**
		 * Serialization to JSON.
		 */
		struct JSON {};
		//struct JSON_PRETTY {};

		/**
		 * Serialization to XML.
		 */
		struct XML {};

		/**
		 * Serialization to CSV.
		 */
		struct CSV {};

		/**
		 * generic serializer
		 */
		template <typename T, typename TAG>
		struct serializer
		{
			static std::size_t write(std::ostream& os, T const& v)
			{
				auto const pos = os.tellp();
				os << v;
				return os.tellp() - pos;
			}
		};


	}
}
#endif
