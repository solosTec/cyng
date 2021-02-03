/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_IO_PARSER_H
#define CYNG_IO_PARSER_H

#include <cyng/obj/intrinsics/container.h>
#include <cyng/obj/intrinsics/buffer.h>
#include <cyng/obj/object.h>
#include <cyng/obj/factory.hpp>
#include <cyng/obj/buffer_cast.hpp>

#include <functional>
#include <algorithm>
//#include <bit>
//#include <cyng.h>	//	docc_BIG_ENDIAN

namespace cyng {
	namespace io {

		/**
		 * Design goal (after many iterations) was to create an utterly simple and
		 * robust parser.
		 */
		class parser
		{
			/**
			 * This enum stores the global state
			 * of the parser. For each state there
			 * are different helper variables mostly
			 * declared in the private section of this class.
			 */
			enum class state
			{
				FAILURE,

				//	t-l-v
				TYPE,
				LENGTH,
				VALUE,

			}	state_;

		public:
			using callback = std::function<void(object&&)>;

		public:
			parser(callback);

			/**
			 * parse the specified range
			 *
			 * @return count of generated instructions
			 */
			template < typename I >
			std::size_t read(I start, I end)
			{
				using value_type = typename std::iterator_traits<I>::value_type;

				std::for_each(start, end, [this](value_type c) {
						this->put(c);
					});
				return end - start;
			}

		private:

			/**
			 * read a single byte and update
			 * parser state.
			 * Implements the state machine
			 */
			void put(char);
			state type(char);
			state length(char);
			state value(char);

			object deserialize();

			template <typename T >
			object deserialize_numeric() {

				BOOST_ASSERT(buffer_.size() == sizeof(T));

				return make_object<T>(to_numeric<T>(buffer_));
			}

			void set_length(std::size_t);


		private:
			callback cb_;

			buffer_t buffer_;
			std::uint16_t	type_;	//!<	type code
			std::uint64_t	length_;	//!<	length
		};

		/**
		 * the first byte in the length field determines the total length of the field
		 */
		constexpr std::size_t size_of_length_field(char c) {

			if (c == 126)
			{
				//	16 bit length
				//	the following 2 bytes interpreted as a 16
				//	bit unsigned integer are the payload length
				return 3u;
			}
			else if (c == 127)
			{
				//	64 bit length
				//	the following 8 bytes interpreted as a 64 - bit unsigned integer(the
				//	most significant bit MUST be 0) are the payload length
				return 9u;
			}
			//	complete (1 byte)
			//	length is less than 126 bytes
			return 1u;
		}

	}
}
#endif
