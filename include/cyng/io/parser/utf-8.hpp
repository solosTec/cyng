/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Sylko Olzscher
 *
 */
#ifndef CYNG_IO_UTF_8_HPP
#define CYNG_IO_UTF_8_HPP

#include <cstdint>
#include <array>
#include <string>
#include <functional>
#include <limits>

#include <boost/assert.hpp>

namespace cyng {

	namespace utf8 {

		/**
		 * @return true if c1 == 0xef, c2 == 0xbb and c3 == 0xbf
		 */
		bool is_bom(char c1, char c2, char c3);

		/**
		 * @brief helper class to convert an utf-8 string to codepoints
		 *
		 */
		class u8_to_u32
		{
		public:
			u8_to_u32();
			u8_to_u32(u8_to_u32 const&) = default;

			/**
			 * Insert next char
			 */
			std::pair<std::uint32_t, bool> put(char c);

			/**
			 * @brief Get the size of UTF-8 character
			 *
			 * @return std::size_t
			 */
			std::size_t size() const;

			std::uint32_t value() const;

		private:
			std::array<std::uint8_t, 4 >  char_;
			std::size_t pos_, size_;
			std::uint32_t value_;

			// we now need to remove a few of the leftmost bits, but how many depends
			// upon how many extra bytes we've extracted:
			static constexpr std::uint32_t masks_[4] =
			{
				0x7Fu,
				0x7FFu,
				0xFFFFu,
				0x1FFFFFu,
			};
		};

		/**
		 * Callback for a complete UTF-8 codepoint as 32 bit integer
		 */
		using emit_u32_f = std::function<void(std::uint32_t)>;

		/**
		 * Reads UTF-8 code points. If one symbol is complete the callback 
		 * will be called.
		 */
		class sanitizer : protected u8_to_u32
		{
		public:
			sanitizer(emit_u32_f);
			void put(char c);

		private:
			emit_u32_f cb_;
		};

		/**
		 * @brief UTF-8 iterator
		 */
		template <typename BaseIterator>
		class u8_to_u32_iterator {

		public:
			using value_type = std::uint32_t;
			using difference_type = std::ptrdiff_t;
			using pointer = std::uint32_t*;
			using reference = std::uint32_t&;
			using iterator_category = std::input_iterator_tag;

		public:
			u8_to_u32_iterator()
				: pos_()
				, value_(std::numeric_limits<std::uint32_t>::max())
			{}

			u8_to_u32_iterator(u8_to_u32_iterator const& i)
				: pos_(i.pos_)
				, value_(i.value_)
			{}

			explicit u8_to_u32_iterator(BaseIterator pos)
				: pos_(pos)
				, value_(std::numeric_limits<std::uint32_t>::max())
			{}

			value_type operator*() const {
				return dereference();
			}

			u8_to_u32_iterator operator++(int)
			{
				u8_to_u32_iterator ret(*this);
				++* this;
				return ret;
			}

			u8_to_u32_iterator& operator++()
			{
				inc();
				return *this;
			}

			bool equals(u8_to_u32_iterator const& pos) const
			{
				return this->pos_ == pos.pos_;
			}

			friend bool operator==(u8_to_u32_iterator const& i1, u8_to_u32_iterator const& i2) {
				return i1.equals(i2);
			}
			friend bool operator!=(u8_to_u32_iterator const& i1, u8_to_u32_iterator const& i2) {
				return !i1.equals(i2);
			}

		private:
			value_type dereference() const {
				if (value_ == std::numeric_limits<std::uint32_t>::max()) {

					BOOST_ASSERT((*pos_ & 0xC0) != 0x80);

					u8_to_u32 conv;
					for (; ; ++pos_) {
						auto const r = conv.put(*pos_);
						if (r.second) {
							value_ = r.first;
							break;
						}
					}
				}
				return value_;
			}

			void inc() const {

				++pos_;
				value_ = std::numeric_limits<std::uint32_t>::max();
			}

		private:
			mutable BaseIterator pos_;
			mutable std::uint32_t	value_;
		};

		/**
		 * @brief helper class to convert codepoints to utf-8 strings
		 *
		 */
		class u32_to_u8
		{
		public:
			u32_to_u8();

			std::pair<char const*, std::uint8_t> put(std::uint32_t c);

		private:
			std::array<char, 4 >  char_;

		};

		template <typename BaseIterator>
		class u32_to_u8_iterator {

		public:
			using value_type = std::string::value_type; //  char
			using difference_type = std::string::difference_type;
			using pointer = std::string::pointer;       //  char*
			using reference = std::string::reference;   //  char&
			using iterator_category = std::input_iterator_tag;

		public:

			u32_to_u8_iterator(u32_to_u8_iterator const& i)
				: pos_(i.pos_)
				, value_{ i.value_ }
				, index_{ i.index_ }
			{}

			u32_to_u8_iterator()
				: pos_()
				, value_{ nullptr, 0u }
				, index_{ 0u }
			{}

			explicit u32_to_u8_iterator(BaseIterator pos)
				: pos_(pos)
				, value_{ nullptr, 0u }
				, index_{ 4u }
			{}

			value_type operator*() const {
				return dereference();
			}

			u32_to_u8_iterator operator++(int)
			{
				u32_to_u8_iterator ret(*this);
				++* this;
				return ret;
			}

			u32_to_u8_iterator& operator++()
			{
				inc();
				return *this;
 			}

			bool equals(u32_to_u8_iterator const& pos) const
			{
				return this->pos_ == pos.pos_;
			}

			friend bool operator==(u32_to_u8_iterator const& i1, u32_to_u8_iterator const& i2) {
				return i1.equals(i2);
			}
			friend bool operator!=(u32_to_u8_iterator const& i1, u32_to_u8_iterator const& i2) {
				return !i1.equals(i2);
			}

		private:
			value_type dereference() const {
	
				if (4 == index_) {
					extract();
				}

				return value_.first[index_];
			}

			void extract() const {
				auto const c = *pos_;
				static u32_to_u8 conv;
				value_ = conv.put(c);
				index_ = 0;
			}

			void inc()
			{
				if (4 == index_) {
					extract();
				}

				//	next surrogate
				++index_;

				if (index_ == value_.second) {
					index_ = 4;
					++pos_;
				}
			}

		private:
			BaseIterator pos_;
			mutable std::pair<char const*, std::uint8_t> value_;
			mutable std::size_t index_;   //  position in string
		};

		/**
		 * @brief get size of an UTF-8 character
		 *
		 * @param c
		 * @return std::uint32_t
		 */
		constexpr std::uint32_t utf8_byte_count(std::uint8_t c)
		{
			switch (c & 0xF0) {
			case 0:       return 0;
			case 0xC0:    return 2;
			case 0xE0:    return 3;
			case 0xF0:    return 4;
			default:
				break;
			}
			return 1;
		}

		/**
		 * @brief get char size
		 *
		 * @param c
		 * @return std::uint8_t
		 */
		constexpr std::uint8_t utf8_char_size(std::uint32_t c)
		{
			if (c < 0x80)   return 1;
			else if (c < 0x800) return 2;
			else if (c < 0x10000) return 3;
			else if (c < 0x200000) return 4;
			//  not standard
			else if (c < 0x4000000) return 5;
			else if (c <= 0x7FFFFFFF) return 6;
			return 0;
		}

		/**
		 * @brief UTF-8 initial byte
		 *
		 * @param size
		 * @return constexpr std::uint8_t
		 */
		constexpr std::uint8_t utf8_prefix(std::uint8_t size)
		{
			switch (size) {
			case 2: return 0xC0;
			case 3: return 0xE0;
			case 4: return 0xF0;
			default:
				break;
			}
			return 0;
		}

	}
}

#endif  //  CYNG_IO_UTF_8_HPP
