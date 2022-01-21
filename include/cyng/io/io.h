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
		 * Serialization to a readable hierarchical, tree-like structure
		 */
		struct PRETTY {};

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
		 * Serialization to C++.
		 * Cenerate C++ source code compatible with this library.
		 */
		struct CPP {};

		/**
		 * RAAI class to get written byte count
		 */
		class calc_size
		{
		public:
			calc_size(std::ostream& os);
			operator std::size_t() const;

		private:
			std::size_t count() const;

		private:
			std::streampos const pos_;
			std::ostream& os_;
		};

		/**
		 * generic serializer
		 */
		template <typename T, typename TAG>
		struct serializer
		{
			static std::size_t write(std::ostream& os, T const& v)
			{
				calc_size const cs(os);
				os << v;
				return cs;
			}
		};

	}
}
#endif
