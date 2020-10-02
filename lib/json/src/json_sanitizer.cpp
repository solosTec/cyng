/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Sylko Olzscher 
 * 
 */ 

#include <cyng/json/json_sanitizer.h>
#include <tuple>
#include <boost/regex/pending/unicode_iterator.hpp>
namespace cyng
{
	namespace json
	{
		sanitizer::sanitizer(emit_token_f cb)
			: cb_(cb)
            , char_{ 0 }
            , pos_{ 0 }
            , extra_{ 0 }
		{
            static_assert(std::tuple_size<decltype(char_)>::value == sizeof(std::uint32_t), "wrong type size");
        }

		bool sanitizer::put(char c)
		{

            if ((c > 0x7Fu) || (extra_ != 0))
            {
                //
                //  initialize glyph
                //
                if (extra_ == 0) {
                    pos_ = 0;
                    extra_ = utf8_byte_count(c);
                }

                //
                //  build code point
                //
                char_[pos_] = static_cast<unsigned char>(c);
                ++pos_;

                //
                //  build glyph
                //
                if (pos_ == extra_) {

                    std::uint32_t val = char_[0];

                    for (std::size_t idx = 1; idx < extra_; ++idx) {
                        val <<= 6;
                        val += static_cast<std::uint8_t>(char_[idx]) & 0x3Fu;
                    }

                    // we now need to remove a few of the leftmost bits, but how many depends
                    // upon how many extra bytes we've extracted:
                    static const std::uint32_t masks[4] =
                    {
                       0x7Fu,
                       0x7FFu,
                       0xFFFFu,
                       0x1FFFFFu,
                    };
                    val &= masks[extra_ - 1];

                    // check the result is in range:
                    //if (val > static_cast<U32Type>(0x10FFFFu))
                    //    invalid_sequence();
                    //// The result must not be a surrogate:
                    //if ((val >= static_cast<U32Type>(0xD800)) && (val <= static_cast<U32Type>(0xDFFF)))
                    //    invalid_sequence();

                    extra_ = 0;
                    cb_(make_token(val));

                    return true;
                }

                return false;
            }

            cb_(make_token(c));
            return true;
		}

        std::uint32_t utf8_byte_count(std::uint8_t c)
        {
            // if the most significant bit with a zero in it is in position
            // 8-N then there are N bytes in this UTF-8 sequence:
            std::uint8_t mask = 0x80u;
            std::uint32_t result = 0;
            while (c & mask)
            {
                ++result;
                mask >>= 1;
            }
            return (result == 0) ? 1 : ((result > 4) ? 4 : result);
        }

        std::uint32_t utf8_trailing_byte_count(std::uint8_t c)
        {
            return utf8_byte_count(c) - 1;
        }
	}
}

