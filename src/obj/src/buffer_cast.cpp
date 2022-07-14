/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#include <cyng/obj/buffer_cast.hpp>
#include <cyng/obj/object_cast.hpp>

namespace cyng {

	buffer_t to_buffer(object const& obj)
	{
		auto p = object_cast<buffer_t>(obj);
		return (p != nullptr)
			? *p
			: buffer_t{}
		;
	}

	buffer_t to_buffer(std::string const& str) {
		return { std::begin(str), std::end(str) };
	}

	std::string to_string_nil(buffer_t const& buffer, std::size_t offset) {
		std::string str;
		if (offset < buffer.size()) {
			str.reserve(buffer.size() - offset);
			for (auto pos = buffer.begin() + offset; pos != buffer.end() && *pos != 0; ++pos) {
				str.push_back(*pos);
			}
		}
		return str;
	}

}

