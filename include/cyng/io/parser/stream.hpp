/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Sylko Olzscher
 *
 */
#ifndef CYNG_IO_STREAM_HPP
#define CYNG_IO_STREAM_HPP

#include <cyng/io/parser/utf-8.hpp>
#include <iterator>
#include <fstream>

#include <boost/assert.hpp>

namespace cyng {

	/**
	 * @return a range to iterate over the specified input (file) stream
	 */
	template <typename I >
	std::pair<I, I>  get_stream_range(std::istream& ifs) {
		using iterator_t = I;
		return { iterator_t(ifs), iterator_t() };
	}

	namespace utf8 {

		/**
		 * @return range of utf-8 iterators
		 */
		template <typename I >
		auto get_utf8_range(I start, I end) -> std::pair<cyng::utf8::u8_to_u32_iterator<I>, cyng::utf8::u8_to_u32_iterator<I>> {
			using base_iterator_t = I;
			using iterator_t = cyng::utf8::u8_to_u32_iterator<I>;

			BOOST_ASSERT(start != end);

			//  skip BOM
			switch (*start & 0xFF) {
			case 0xEF:
				if (((*++start & 0xFF) == 0xBB) && ((*++start & 0xFF) == 0xBF)) {
					++start;
				}
				else {
					//  error
				}
				break;
			case 0xFE:
				//  error: utf-16 BE
				BOOST_ASSERT_MSG(false, "utf-16 BE");
				return { iterator_t(end), iterator_t(end) };
			case 0xFF:
				//  error: utf-16 LE
				BOOST_ASSERT_MSG(false, "utf-16 LE");
				return { iterator_t(end), iterator_t(end) };
			default:
				break;
			}

			return { iterator_t(start), iterator_t(end) };
		}

		template <typename I >
		auto get_utf8_range(std::istream& ifs) -> std::pair<cyng::utf8::u8_to_u32_iterator<I>, cyng::utf8::u8_to_u32_iterator<I>> {
			auto [start, end] = get_stream_range<I>(ifs);
			return get_utf8_range<I>(start, end);
		}
	}
}

#endif  //  CYNG_IO_UTF_8_H
