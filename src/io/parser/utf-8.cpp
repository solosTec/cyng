#include <cyng/io/parser/utf-8.h>

namespace cyng {
    namespace utf8 {
   
        u8_to_u32::u8_to_u32()
        : char_{ 0 }
            , pos_{ 0 }
            , size_{ 0 }
            , value_{ 0 }
            {}

        std::size_t u8_to_u32::size() const
        {
            return size_;
        }

        std::uint32_t u8_to_u32::value() const
        {
            return value_;
        }

        std::pair<std::uint32_t, bool> u8_to_u32::put(char c)
        {
            if ((c > 0x7Fu) || (size_ != 0))
            {
                //
                //  initialize glyph
                //
                if (size_ == 0) {
                    pos_ = 0;
                    size_ = utf8_byte_count(c);
                }

                //
                //  build code point
                //
                char_[pos_] = static_cast<unsigned char>(c);
                ++pos_;

                //
                //  build glyph
                //
                if (pos_ == size_) {

                    value_ = char_[0];

                    for (std::size_t idx = 1; idx < size_; ++idx) {
                        value_ <<= 6;
                        value_ += static_cast<std::uint8_t>(char_[idx]) & 0x3Fu;
                    }

                    // we now need to remove a few of the leftmost bits, but how many depends
                    // upon how many extra bytes we've extracted:
                    value_ &= masks_[size_ - 1];


                    size_ = 0;
                    return { value_, true} ;
                }

                return { c, false };
            }

            value_ = c;
            return { value_, true };
        }    

        sanitizer::sanitizer(emit_u32_f cb)
            : cb_(cb)
        {
            BOOST_ASSERT(cb_);
        }

        void sanitizer::put(char c) {
            auto const r = u8_to_u32::put(c);
            if (r.second)   cb_(r.first);
        }


        u32_to_u8::u32_to_u8()
        : char_{ 0 }
            {}

        std::pair<char const*, std::uint8_t> u32_to_u8::put(std::uint32_t c)
        {
            auto const size = utf8_char_size(c);
            BOOST_ASSERT(size < 5);

            for(auto pos = char_.size(); pos > 1; --pos)    {
                if (pos <= size) {
                    char_[pos - 1] = (c & 0x3F) | 0x80;
                    c = c >> 6;
                }
                else {
                    char_[pos - 1] = '\0';
                }
            }

            char_[0] = c | utf8_prefix(size);
            return { char_.data(), size };
        }

    }
}
