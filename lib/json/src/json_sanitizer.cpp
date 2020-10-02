/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Sylko Olzscher 
 * 
 */ 

#include <cyng/json/json_sanitizer.h>
#include <tuple>

namespace cyng
{
	namespace json
	{
		sanitizer::sanitizer(emit_token_f cb)
			: cb_(cb)
            , char_{ 0 }
            , pos_{ 0 }
		{
            static_assert(std::tuple_size<decltype(char_)>::value == sizeof(std::uint32_t), "wrong type size");
        }

		bool sanitizer::put(char c)
		{
            // if (c > 0x10FFFFu) detail::invalid_utf32_code_point(c);
            if (c > 0x7Fu)
            {
                char_[pos_] = c;
                ++pos_;
                //*m_position++ = static_cast<unsigned char>(c);
             } 
            else {
                pos_ = 0;
                char_[pos_] = c;
            }
            //else if (c < 0x800u)
            //{
                //*m_position++ = static_cast<unsigned char>(0xC0u + (c >> 6));
                //*m_position++ = static_cast<unsigned char>(0x80u + (c & 0x3Fu));
            //}
            //else if (c < 0x10000u)
            //{
                //*m_position++ = static_cast<unsigned char>(0xE0u + (c >> 12));
                //*m_position++ = static_cast<unsigned char>(0x80u + ((c >> 6) & 0x3Fu));
                //*m_position++ = static_cast<unsigned char>(0x80u + (c & 0x3Fu));
            //}
            //else
            //{
                //*m_position++ = static_cast<unsigned char>(0xF0u + (c >> 18));
                //*m_position++ = static_cast<unsigned char>(0x80u + ((c >> 12) & 0x3Fu));
                //*m_position++ = static_cast<unsigned char>(0x80u + ((c >> 6) & 0x3Fu));
                //*m_position++ = static_cast<unsigned char>(0x80u + (c & 0x3Fu));
            //}

			cb_(make_token(*reinterpret_cast<std::uint32_t*>(char_.data())));
			return true;
		}

	}
}

